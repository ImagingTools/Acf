// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <idoc/CFileSerializedUndoManagerComp.h>


// Qt includes
#include <QtCore/QCryptographicHash>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace idoc
{


// CFileUndoState

CFileSerializedUndoManagerComp::CFileUndoState::CFileUndoState(const QString& filePath)
:	m_filePath(filePath)
{
}


CFileSerializedUndoManagerComp::CFileUndoState::~CFileUndoState()
{
	if (!m_filePath.isEmpty() && QFile::exists(m_filePath)){
		QFile::remove(m_filePath);
	}
}


const QString& CFileSerializedUndoManagerComp::CFileUndoState::GetFilePath() const
{
	return m_filePath;
}


qint64 CFileSerializedUndoManagerComp::CFileUndoState::GetStateSize() const
{
	return QFileInfo(m_filePath).size();
}


bool CFileSerializedUndoManagerComp::CFileUndoState::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag filePathTag("FilePath", "Path to the file storing the document state", iser::CArchiveTag::TT_LEAF);

	return archive.BeginTag(filePathTag) && archive.Process(m_filePath) && archive.EndTag(filePathTag);
}


// CFileSerializedUndoManagerComp

CFileSerializedUndoManagerComp::CFileSerializedUndoManagerComp()
:	m_uniqueFileCounter(0)
{
}


// reimplemented (iser::ISerializable)

bool CFileSerializedUndoManagerComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag currentStepTag("CurrentStep", "Index of the current undo step", iser::CArchiveTag::TT_LEAF);
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

		int stepsCount = undoList.size() + redoList.size();

		retVal = retVal && archive.BeginMultiTag(stepsTag, stepTag, stepsCount);

		for (int listIndex = 0; (listIndex < 2) && retVal; ++listIndex){
			UndoList& list = (listIndex == 0)? undoList: redoList;

			for (UndoList::iterator iter = list.begin(); iter != list.end(); ++iter){
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
		}

		retVal = retVal && archive.EndTag(stepsTag);
	}
	else{
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		qint32 currentStepIndex = 0;
		retVal = retVal && archive.TagAndProcess(currentStepTag, currentStepIndex);

		int stepsCount = 0;
		retVal = retVal && archive.BeginMultiTag(stepsTag, stepTag, stepsCount);
		if (!retVal){
			return false;
		}

		undoList.clear();
		redoList.clear();

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
				UndoList& targetList = (stepIndex < currentStepIndex)? undoList: redoList;

				targetList.push_back(UndoStepInfo());
				targetList.back().description = description;
				targetList.back().statePtr.TakeOver(statePtr);
			}
		}

		retVal = retVal && archive.EndTag(stepsTag);
	}

	return retVal;
}


// protected methods

QString CFileSerializedUndoManagerComp::CreateStepFilePath(int stepIndex) const
{
	if (!m_directoryCompPtr.IsValid()){
		return QString();
	}

	QDir directory(m_directoryCompPtr->GetPath());

	return directory.filePath(QString("step_%1.bin").arg(stepIndex));
}


// reimplemented (CSerializedUndoManagerCompBase)

CSerializedUndoManagerCompBase::IUndoState* CFileSerializedUndoManagerComp::CreateState(iser::ISerializable& object)
{
	if (!m_persistenceCompPtr.IsValid() || !m_directoryCompPtr.IsValid()){
		return NULL;
	}

	int currentStepIndex = GetAvailableUndoSteps();

	QString filePath = CreateStepFilePath(currentStepIndex);
	if (filePath.isEmpty()){
		return NULL;
	}

	// The base class temporarily creates additional states for comparison while a state for the
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


bool CFileSerializedUndoManagerComp::RestoreState(const IUndoState& state, iser::ISerializable& object)
{
	if (!m_persistenceCompPtr.IsValid()){
		return false;
	}

	const CFileUndoState& fileState = static_cast<const CFileUndoState&>(state);

	return m_persistenceCompPtr->LoadFromFile(object, fileState.GetFilePath()) == ifile::IFilePersistence::OS_OK;
}


bool CFileSerializedUndoManagerComp::AreStatesEqual(const IUndoState& state1, const IUndoState& state2) const
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


} // namespace idoc
