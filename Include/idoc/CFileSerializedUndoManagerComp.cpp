// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <idoc/CFileSerializedUndoManagerComp.h>


// Qt includes
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>


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


// CFileSerializedUndoManagerComp

CFileSerializedUndoManagerComp::CFileSerializedUndoManagerComp()
:	m_nextStepIndex(0)
{
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

	QString filePath = CreateStepFilePath(m_nextStepIndex);
	if (filePath.isEmpty()){
		return NULL;
	}

	if (m_persistenceCompPtr->SaveToFile(object, filePath) != ifile::IFilePersistence::OS_OK){
		return NULL;
	}

	++m_nextStepIndex;

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

	return file1.readAll() == file2.readAll();
}


void CFileSerializedUndoManagerComp::OnUndoPositionChanged()
{
	if (m_currentStepCompPtr.IsValid()){
		m_currentStepCompPtr->SetSelectedOptionIndex(GetAvailableUndoSteps());
	}
}


} // namespace idoc
