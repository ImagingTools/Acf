// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <idoc/CFileBasedUndoManagerComp.h>


// Qt includes
#include <QtCore/QCryptographicHash>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <iser/CMemoryReadArchive.h>
#include <icomp/CComponentBase.h>


namespace idoc
{


// static attributes
static const istd::IChangeable::ChangeSet s_undoChangeSet(istd::IChangeable::CF_NO_UNDO, "UNDO");


// CFileUndoState

CFileBasedUndoManagerComp::CFileUndoState::CFileUndoState(const QString& filePath)
:	m_filePath(filePath)
{
}


CFileBasedUndoManagerComp::CFileUndoState::~CFileUndoState()
{
	if (!m_filePath.isEmpty() && QFile::exists(m_filePath)){
		QFile::remove(m_filePath);
	}
}


const QString& CFileBasedUndoManagerComp::CFileUndoState::GetFilePath() const
{
	return m_filePath;
}


qint64 CFileBasedUndoManagerComp::CFileUndoState::GetStateSize() const
{
	return QFileInfo(m_filePath).size();
}


bool CFileBasedUndoManagerComp::CFileUndoState::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag filePathTag("FilePath", "Path to the file storing the document state", iser::CArchiveTag::TT_LEAF);

	return archive.BeginTag(filePathTag) && archive.Process(m_filePath) && archive.EndTag(filePathTag);
}


// CFileBasedUndoManagerComp

CFileBasedUndoManagerComp::CFileBasedUndoManagerComp()
:	m_uniqueFileCounter(0),
	m_hasStoredDocumentState(false),
	m_isBlocked(false),
	m_stateChangedFlag(DCF_UNKNOWN),
	m_isStateChangedFlagValid(false)
{
}


// reimplemented (idoc::IUndoManager)

int CFileBasedUndoManagerComp::GetAvailableUndoSteps() const
{
	return m_undoList.size();
}


int CFileBasedUndoManagerComp::GetAvailableRedoSteps() const
{
	return m_redoList.size();
}


QString CFileBasedUndoManagerComp::GetUndoLevelDescription(int stepIndex) const
{
	if ((stepIndex > 0) && (stepIndex <= m_undoList.size())){
		return m_undoList[m_undoList.size() - stepIndex].description;
	}

	return "";
}


QString CFileBasedUndoManagerComp::GetRedoLevelDescription(int stepIndex) const
{
	if ((stepIndex > 0) && (stepIndex <= m_redoList.size())){
		return m_redoList[m_redoList.size() - stepIndex].description;
	}

	return "";
}


void CFileBasedUndoManagerComp::ResetUndo()
{
	istd::CChangeNotifier notifier(this);
	Q_UNUSED(notifier);

	m_undoList.clear();
	m_redoList.clear();
	m_beginStatePtr.Reset();
}


bool CFileBasedUndoManagerComp::DoUndo(int steps)
{
	return DoListShift(steps, m_undoList, m_redoList);
}


bool CFileBasedUndoManagerComp::DoRedo(int steps)
{
	return DoListShift(steps, m_redoList, m_undoList);
}


// reimplemented (imod::IObserver)

bool CFileBasedUndoManagerComp::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	if (BaseClass2::OnModelAttached(modelPtr, changeMask)){
		m_hasStoredDocumentState = false;
		m_storedStateArchive.Reset();

		m_stateChangedFlag = DCF_UNKNOWN;

		return true;
	}

	return false;
}


bool CFileBasedUndoManagerComp::OnModelDetached(imod::IModel* modelPtr)
{
	if (BaseClass2::OnModelDetached(modelPtr)){
		m_hasStoredDocumentState = false;
		m_storedStateArchive.Reset();

		m_stateChangedFlag = DCF_UNKNOWN;

		return true;
	}

	return false;
}


// reimplemented (iser::ISerializable)

bool CFileBasedUndoManagerComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag currentStepTag("CurrentStep", "Index of the current undo step", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag hasCurrentStateTag("HasCurrentState", "Flag indicating whether the observed document state at the current step is stored as a step file", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag stepsTag("Steps", "List of all stored undo and redo steps", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag stepTag("Step", "Single undo or redo step", iser::CArchiveTag::TT_GROUP, &stepsTag);
	static iser::CArchiveTag descriptionTag("Description", "Human readable step description", iser::CArchiveTag::TT_LEAF, &stepTag);
	static iser::CArchiveTag stateTag("State", "Stored document state", iser::CArchiveTag::TT_GROUP, &stepTag);

	UndoList& undoList = m_undoList;
	UndoList& redoList = m_redoList;

	bool retVal = true;

	if (archive.IsStoring()){
		qint32 currentStepIndex = qint32(undoList.size());

		retVal = retVal && archive.TagAndProcess(currentStepTag, currentStepIndex);

		// Instead of writing the observed document into the archive, its state at the current step is
		// stored in a dedicated step file placed between the undo and the redo steps. On loading it is
		// restored back from the file corresponding to the current step.
		m_currentStatePtr.Reset();

		iser::ISerializable* observedObjectPtr = GetObservedObject();
		if (observedObjectPtr != NULL){
			IUndoState* currentStatePtr = CreateState(*observedObjectPtr);
			if (currentStatePtr != NULL){
				m_currentStatePtr.SetPtr(currentStatePtr);
			}
		}

		bool hasCurrentState = m_currentStatePtr.IsValid();

		retVal = retVal && archive.TagAndProcess(hasCurrentStateTag, hasCurrentState);

		int stepsCount = undoList.size() + redoList.size() + (hasCurrentState? 1: 0);

		retVal = retVal && archive.BeginMultiTag(stepsTag, stepTag, stepsCount);

		// The steps are written as a single flat list ordered as: undo steps, the current step (if any),
		// then the redo steps, so that the entry at currentStepIndex holds the current document state.
		for (UndoList::iterator iter = undoList.begin(); (iter != undoList.end()) && retVal; ++iter){
			CFileUndoState* fileStatePtr = static_cast<CFileUndoState*>(iter->statePtr.GetPtr());
			if (fileStatePtr == NULL){
				return false;
			}

			retVal = retVal && archive.BeginTag(stepTag);
			retVal = retVal && archive.TagAndProcess(descriptionTag, iter->description);
			retVal = retVal && archive.BeginTag(stateTag);
			retVal = retVal && fileStatePtr->Serialize(archive);
			retVal = retVal && archive.EndTag(stateTag);
			retVal = retVal && archive.EndTag(stepTag);
		}

		if (retVal && hasCurrentState){
			CFileUndoState* currentFileStatePtr = static_cast<CFileUndoState*>(m_currentStatePtr.GetPtr());

			QString currentDescription;

			retVal = retVal && archive.BeginTag(stepTag);
			retVal = retVal && archive.TagAndProcess(descriptionTag, currentDescription);
			retVal = retVal && archive.BeginTag(stateTag);
			retVal = retVal && currentFileStatePtr->Serialize(archive);
			retVal = retVal && archive.EndTag(stateTag);
			retVal = retVal && archive.EndTag(stepTag);
		}

		for (UndoList::iterator iter = redoList.begin(); (iter != redoList.end()) && retVal; ++iter){
			CFileUndoState* fileStatePtr = static_cast<CFileUndoState*>(iter->statePtr.GetPtr());
			if (fileStatePtr == NULL){
				return false;
			}

			retVal = retVal && archive.BeginTag(stepTag);
			retVal = retVal && archive.TagAndProcess(descriptionTag, iter->description);
			retVal = retVal && archive.BeginTag(stateTag);
			retVal = retVal && fileStatePtr->Serialize(archive);
			retVal = retVal && archive.EndTag(stateTag);
			retVal = retVal && archive.EndTag(stepTag);
		}

		retVal = retVal && archive.EndTag(stepsTag);
	}
	else{
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		qint32 currentStepIndex = 0;
		retVal = retVal && archive.TagAndProcess(currentStepTag, currentStepIndex);

		bool hasCurrentState = false;
		retVal = retVal && archive.TagAndProcess(hasCurrentStateTag, hasCurrentState);

		int stepsCount = 0;
		retVal = retVal && archive.BeginMultiTag(stepsTag, stepTag, stepsCount);
		if (!retVal){
			return false;
		}

		undoList.clear();
		redoList.clear();
		m_currentStatePtr.Reset();

		for (int stepIndex = 0; (stepIndex < stepsCount) && retVal; ++stepIndex){
			QString description;

			CFileUndoState* fileStatePtr = new CFileUndoState(QString());
			UndoStatePtr statePtr(fileStatePtr);

			retVal = retVal && archive.BeginTag(stepTag);
			retVal = retVal && archive.TagAndProcess(descriptionTag, description);
			retVal = retVal && archive.BeginTag(stateTag);
			retVal = retVal && fileStatePtr->Serialize(archive);
			retVal = retVal && archive.EndTag(stateTag);
			retVal = retVal && archive.EndTag(stepTag);

			if (retVal){
				if (hasCurrentState && (stepIndex == currentStepIndex)){
					// The step at the current index holds the observed document state at that step.
					m_currentStatePtr.TakeOver(statePtr);
				}
				else{
					UndoList& targetList = (stepIndex < currentStepIndex)? undoList: redoList;

					targetList.push_back(UndoStepInfo());
					targetList.back().description = description;
					targetList.back().statePtr.TakeOver(statePtr);
				}
			}
		}

		retVal = retVal && archive.EndTag(stepsTag);

		if (retVal && m_currentStatePtr.IsValid()){
			// Bring the observed document back to the content it had at the current step by loading it
			// from the file corresponding to that step.
			RestoreObservedObject(*m_currentStatePtr);
		}
	}

	return retVal;
}


// protected methods

bool CFileBasedUndoManagerComp::DoListShift(int steps, UndoList& fromList, UndoList& toList)
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

	return retVal;
}


CFileBasedUndoManagerComp::IUndoState* CFileBasedUndoManagerComp::CreateState(iser::ISerializable& object)
{
	if (!m_persistenceCompPtr.IsValid() || !m_directoryCompPtr.IsValid()){
		return NULL;
	}

	int currentStepIndex = GetAvailableUndoSteps();

	QString filePath = CreateStepFilePath(currentStepIndex);
	if (filePath.isEmpty()){
		return NULL;
	}

	// The undo manager temporarily creates additional states for comparison while a state for the
	// same step already exists, so a unique backing file is chosen to avoid collisions.
	QDir directory(m_directoryCompPtr->GetPath());
	while (QFile::exists(filePath)){
		filePath = directory.filePath(QString("step_%1_%2.bin").arg(currentStepIndex).arg(m_uniqueFileCounter++));
	}

	if (m_persistenceCompPtr->SaveToFile(object, filePath) != ifile::IFilePersistence::OS_OK){
		return NULL;
	}

	return new CFileUndoState(filePath);
}


bool CFileBasedUndoManagerComp::RestoreState(const IUndoState& state, iser::ISerializable& object)
{
	if (!m_persistenceCompPtr.IsValid()){
		return false;
	}

	const CFileUndoState& fileState = static_cast<const CFileUndoState&>(state);

	return m_persistenceCompPtr->LoadFromFile(object, fileState.GetFilePath()) == ifile::IFilePersistence::OS_OK;
}


bool CFileBasedUndoManagerComp::AreStatesEqual(const IUndoState& state1, const IUndoState& state2) const
{
	const CFileUndoState& fileState1 = static_cast<const CFileUndoState&>(state1);
	const CFileUndoState& fileState2 = static_cast<const CFileUndoState&>(state2);

	QFile file1(fileState1.GetFilePath());
	QFile file2(fileState2.GetFilePath());

	if (!file1.open(QIODevice::ReadOnly) || !file2.open(QIODevice::ReadOnly)){
		return false;
	}

	if (file1.size() != file2.size()){
		return false;
	}

	QCryptographicHash hash1(QCryptographicHash::Sha256);
	QCryptographicHash hash2(QCryptographicHash::Sha256);

	if (!hash1.addData(&file1) || !hash2.addData(&file2)){
		return false;
	}

	return hash1.result() == hash2.result();
}


bool CFileBasedUndoManagerComp::RestoreObservedObject(const IUndoState& state)
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


QString CFileBasedUndoManagerComp::CreateStepFilePath(int stepIndex) const
{
	if (!m_directoryCompPtr.IsValid()){
		return QString();
	}

	QDir directory(m_directoryCompPtr->GetPath());

	return directory.filePath(QString("step_%1.bin").arg(stepIndex));
}


// reimplemented (imod::TSingleModelObserverBase<iser::ISerializable>)

iser::ISerializable* CFileBasedUndoManagerComp::CastFromModel(imod::IModel* modelPtr) const
{
	return CompCastPtr<iser::ISerializable>(modelPtr);
}


// reimplemented (imod::IObserver)

void CFileBasedUndoManagerComp::BeforeUpdate(imod::IModel* modelPtr)
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


void CFileBasedUndoManagerComp::AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet)
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

bool CFileBasedUndoManagerComp::HasStoredDocumentState() const
{
	return m_hasStoredDocumentState;
}


bool CFileBasedUndoManagerComp::StoreDocumentState()
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


bool CFileBasedUndoManagerComp::RestoreDocumentState()
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
	}

	return false;
}


IDocumentStateComparator::DocumentChangeFlag CFileBasedUndoManagerComp::GetDocumentChangeFlag() const
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

void CFileBasedUndoManagerComp::OnComponentDestroyed()
{
	EnsureModelDetached();

	BaseClass::OnComponentDestroyed();
}


// private methods

qint64 CFileBasedUndoManagerComp::GetUsedMemorySize() const
{
	qint64 memorySize = 0;

	for (UndoList::ConstIterator iter = m_undoList.constBegin(); iter != m_undoList.constEnd(); ++iter){
		memorySize += iter->statePtr->GetStateSize();
	}

	return memorySize;
}


} // namespace idoc
