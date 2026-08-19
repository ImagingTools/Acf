// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <idoc/CSerializedUndoManagerCompBase.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/CMemoryReadArchive.h>
#include <icomp/CComponentBase.h>


namespace idoc
{


// static attributes
static const istd::IChangeable::ChangeSet s_undoChangeSet(istd::IChangeable::CF_NO_UNDO, "UNDO");


CSerializedUndoManagerCompBase::CSerializedUndoManagerCompBase()
:	m_hasStoredDocumentState(false),
	m_isBlocked(false),
	m_stateChangedFlag(DCF_UNKNOWN),
	m_isStateChangedFlagValid(false)
{
}


// reimplemented (idoc::IUndoManager)

int CSerializedUndoManagerCompBase::GetAvailableUndoSteps() const
{
	return m_undoList.size();
}


int CSerializedUndoManagerCompBase::GetAvailableRedoSteps() const
{
	return m_redoList.size();
}


QString CSerializedUndoManagerCompBase::GetUndoLevelDescription(int stepIndex) const
{
	if ((stepIndex > 0) && (stepIndex <= m_undoList.size())){
		return m_undoList[m_undoList.size() - stepIndex].description;
	}

	return "";
}


QString CSerializedUndoManagerCompBase::GetRedoLevelDescription(int stepIndex) const
{
	if ((stepIndex > 0) && (stepIndex <= m_redoList.size())){
		return m_redoList[m_redoList.size() - stepIndex].description;
	}

	return "";
}


void CSerializedUndoManagerCompBase::ResetUndo()
{
	istd::CChangeNotifier notifier(this);
	Q_UNUSED(notifier);

	m_undoList.clear();
	m_redoList.clear();
	m_beginStatePtr.Reset();

	OnUndoPositionChanged();
}


bool CSerializedUndoManagerCompBase::DoUndo(int steps)
{
	return DoListShift(steps, m_undoList, m_redoList);
}


bool CSerializedUndoManagerCompBase::DoRedo(int steps)
{
	return DoListShift(steps, m_redoList, m_undoList);
}


// reimplemented (imod::IObserver)

bool CSerializedUndoManagerCompBase::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	if (BaseClass2::OnModelAttached(modelPtr, changeMask)){
		m_hasStoredDocumentState = false;
		m_storedStateArchive.Reset();

		m_stateChangedFlag = DCF_UNKNOWN;

		return true;
	}

	return false;
}


bool CSerializedUndoManagerCompBase::OnModelDetached(imod::IModel* modelPtr)
{
	if (BaseClass2::OnModelDetached(modelPtr)){
		m_hasStoredDocumentState = false;
		m_storedStateArchive.Reset();

		m_stateChangedFlag = DCF_UNKNOWN;

		return true;
	}

	return false;
}


// protected methods

bool CSerializedUndoManagerCompBase::DoListShift(int steps, UndoList& fromList, UndoList& toList)
{
	bool retVal = false;

	if ((steps > 0) && (fromList.size() >= steps)){
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		iser::ISerializable* objectPtr = GetObservedObject();

		if (objectPtr != NULL){
			Q_ASSERT(!m_isBlocked);
			m_isBlocked = true;

			istd::CChangeNotifier objectNotifier(objectPtr, &s_undoChangeSet);
			Q_UNUSED(objectNotifier);

			// stores pointer to last processed description container, necessary due of description shift
			// in undo list the state corresponds to state before changes, in redo - after changes. It causes description shift.
			QString* lastDescriptionPtr = NULL;

			UndoStatePtr currentStatePtr(CreateState(*objectPtr));
			if (currentStatePtr.IsValid()){
				toList.push_back(UndoStepInfo());

				UndoStepInfo& currentStep = toList.back();

				currentStep.statePtr.TakeOver(currentStatePtr);
				lastDescriptionPtr = &currentStep.description;
			}

			const UndoStatePtr& sourceStatePtr = fromList[fromList.size() - steps].statePtr;
			Q_ASSERT(sourceStatePtr.IsValid());

			if (RestoreState(*sourceStatePtr, *objectPtr)){
				for (int i = 1; i < steps; ++i){
					if (lastDescriptionPtr != NULL){
						*lastDescriptionPtr = fromList.back().description;
					}

					toList.push_back(UndoStepInfo());
					toList.back().statePtr.TakeOver(fromList.back().statePtr);
					lastDescriptionPtr = &toList.back().description;

					fromList.pop_back();
				}

				if (lastDescriptionPtr != NULL){
					*lastDescriptionPtr = fromList.back().description;
				}

				fromList.pop_back();

				retVal = true;
			}

			objectNotifier.Reset();

			m_isBlocked = false;
		}
	}

	if (retVal){
		OnUndoPositionChanged();
	}

	return retVal;
}


void CSerializedUndoManagerCompBase::OnUndoPositionChanged()
{
	// Default implementation does nothing.
}


bool CSerializedUndoManagerCompBase::RestoreObservedObject(const IUndoState& state)
{
	iser::ISerializable* objectPtr = GetObservedObject();
	if (objectPtr == NULL){
		return false;
	}

	Q_ASSERT(!m_isBlocked);
	m_isBlocked = true;

	istd::CChangeNotifier objectNotifier(objectPtr, &s_undoChangeSet);
	Q_UNUSED(objectNotifier);

	bool retVal = RestoreState(state, *objectPtr);

	objectNotifier.Reset();

	m_isBlocked = false;

	return retVal;
}


// reimplemented (imod::TSingleModelObserverBase<iser::ISerializable>)

iser::ISerializable* CSerializedUndoManagerCompBase::CastFromModel(imod::IModel* modelPtr) const
{
	return CompCastPtr<iser::ISerializable>(modelPtr);
}


// reimplemented (imod::IObserver)

void CSerializedUndoManagerCompBase::BeforeUpdate(imod::IModel* modelPtr)
{
	BaseClass2::BeforeUpdate(modelPtr);

	if (!m_isBlocked && !m_beginStatePtr.IsValid()){
		iser::ISerializable* objectPtr = GetObservedObject();
		if (objectPtr != NULL){
			UndoStatePtr statePtr(CreateState(*objectPtr));

			if (		statePtr.IsValid() &&
						(m_undoList.isEmpty() || !AreStatesEqual(*statePtr, *(m_undoList.back().statePtr)))){
				m_beginStatePtr.TakeOver(statePtr);
			}
		}
	}
}


void CSerializedUndoManagerCompBase::AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet)
{
	Q_ASSERT(!changeSet.IsEmpty());

	m_isStateChangedFlagValid = false;

	bool skipUndo = changeSet.ContainsExplicit(istd::IChangeable::CF_NO_UNDO, true);

	if (		!m_isBlocked &&
				!skipUndo &&
				m_beginStatePtr.IsValid()){
		iser::ISerializable* objectPtr = GetObservedObject();
		if (objectPtr != NULL){
			UndoStatePtr statePtr(CreateState(*objectPtr));

			if (statePtr.IsValid()){
				if (!AreStatesEqual(*statePtr, *m_beginStatePtr)){
					istd::CChangeNotifier notifier(this);
					Q_UNUSED(notifier);

					m_undoList.push_back(UndoStepInfo());
					m_undoList.back().statePtr.TakeOver(m_beginStatePtr);
					m_undoList.back().description = changeSet.GetDescription();

					if (m_maxBufferSizeAttrPtr.IsValid() && (GetUsedMemorySize() > qint64(*m_maxBufferSizeAttrPtr) * (1 << 20))){
						m_undoList.pop_front();
					}

					m_redoList.clear();

					OnUndoPositionChanged();
				}
			}
			else{
				qDebug("Undo Manager: Object serialization failed");
			}
		}

		m_beginStatePtr.Reset();
	}

	BaseClass2::AfterUpdate(modelPtr, changeSet);
}


// reimplemented (idoc::IDocumentStateComparator)

bool CSerializedUndoManagerCompBase::HasStoredDocumentState() const
{
	return m_hasStoredDocumentState;
}


bool CSerializedUndoManagerCompBase::StoreDocumentState()
{
	istd::CChangeNotifier notifier(this);
	Q_UNUSED(notifier);

	m_storedStateArchive.Reset();

	iser::ISerializable* serializablePtr = GetObservedObject();
	if ((serializablePtr != NULL) && serializablePtr->Serialize(m_storedStateArchive)){
		m_stateChangedFlag = DCF_EQUAL;
		m_hasStoredDocumentState = true;
	}
	else{
		m_stateChangedFlag = DCF_UNKNOWN;
		m_hasStoredDocumentState = false;
	}
	m_isStateChangedFlagValid = true;

	return m_hasStoredDocumentState;
}


bool CSerializedUndoManagerCompBase::RestoreDocumentState()
{
	iser::CMemoryReadArchive restoreArchive(m_storedStateArchive);

	if (m_hasStoredDocumentState){
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		iser::ISerializable* objectPtr = GetObservedObject();
		if (objectPtr != NULL){
			Q_ASSERT(!m_isBlocked);
			m_isBlocked = true;

			if (objectPtr->Serialize(restoreArchive)){
				m_stateChangedFlag = DCF_EQUAL;
				m_isStateChangedFlagValid = true;

				m_isBlocked = false;

				return true;
			}

			m_isBlocked = false;
		}

		m_stateChangedFlag = DCF_UNKNOWN;
		m_isStateChangedFlagValid = true;

		m_undoList.clear();
		m_redoList.clear();

		OnUndoPositionChanged();
	}

	return false;
}


IDocumentStateComparator::DocumentChangeFlag CSerializedUndoManagerCompBase::GetDocumentChangeFlag() const
{
	if (!m_isStateChangedFlagValid){
		m_stateChangedFlag = DCF_UNKNOWN;

		if (m_hasStoredDocumentState){
			iser::CMemoryWriteArchive compareArchive;

			iser::ISerializable* serializablePtr = GetObservedObject();
			if ((serializablePtr != NULL) && const_cast<iser::ISerializable*>(serializablePtr)->Serialize(compareArchive)){
				m_stateChangedFlag = (compareArchive != m_storedStateArchive)? DCF_DIFFERENT: DCF_EQUAL;
			}
		}

		m_isStateChangedFlagValid = true;
	}

	return m_stateChangedFlag;
}


// reimplemented (icomp::CComponentBase)

void CSerializedUndoManagerCompBase::OnComponentDestroyed()
{
	EnsureModelDetached();

	BaseClass::OnComponentDestroyed();
}


// private methods

qint64 CSerializedUndoManagerCompBase::GetUsedMemorySize() const
{
	qint64 memorySize = 0;

	for (UndoList::ConstIterator iter = m_undoList.constBegin(); iter != m_undoList.constEnd(); ++iter){
		memorySize += iter->statePtr->GetStateSize();
	}

	return memorySize;
}


} // namespace idoc
