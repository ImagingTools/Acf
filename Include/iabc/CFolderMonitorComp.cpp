#include "iabc/CFolderMonitorComp.h"


// Qt includes
#include <QDir>
#include <QApplication>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "imod/IModel.h"

#include "iser/IArchive.h"
#include "iser/CArchivetag.h"

#include "isys/CSectionBlocker.h"

#include "iqt/CTimer.h"


namespace iabc
{


CFolderMonitorComp::CFolderMonitorComp()
	:m_notificationFrequency(10),
	m_poolingFrequency(60),
	m_finishThread(false),
	m_fileSystemWatcher(this)
{
}


// reimplemented (iabc::IDirectoryMonitor)

istd::CStringList CFolderMonitorComp::GetChangedFileItems(int changeFlags) const
{
	isys::CSectionBlocker block(&m_lock);
	
	istd::CStringList changedFilesList;
	if ((changeFlags & FilesAdded) != 0){
		istd::CStringList addedFiles = iqt::GetCStringList(m_folderChanges.addedFiles);
		changedFilesList.insert(changedFilesList.end(), addedFiles.begin(), addedFiles.end()); 
	}

	if ((changeFlags & FilesRemoved) != 0){
		istd::CStringList removedFiles = iqt::GetCStringList(m_folderChanges.removedFiles);
		changedFilesList.insert(changedFilesList.end(), removedFiles.begin(), removedFiles.end()); 
	}

	if ((changeFlags & FilesModified) != 0){
		istd::CStringList modifiedFiles = iqt::GetCStringList(m_folderChanges.modifiedFiles);
		changedFilesList.insert(changedFilesList.end(), modifiedFiles.begin(), modifiedFiles.end()); 
	}

	if ((changeFlags & FilesAttributeChanged) != 0){
		istd::CStringList attributeChangedFiles = iqt::GetCStringList(m_folderChanges.attributeChangedFiles);
		changedFilesList.insert(changedFilesList.end(), attributeChangedFiles.begin(), attributeChangedFiles.end()); 
	}

	return changedFilesList;
}


// reimplemented (ibase::IFileListProvider)

istd::CStringList CFolderMonitorComp::GetFileList() const
{
	istd::CStringList fileList;

	for (int fileIndex = 0; fileIndex < m_directoryFiles.count(); fileIndex++){
		fileList.push_back(iqt::GetCString(m_directoryFiles[fileIndex].absoluteFilePath()));
	}

	return fileList;
}


// reimplemented (iprm::IFileNameParam)

int CFolderMonitorComp::GetPathType() const
{
	return iprm::IFileNameParam::PT_DIRECTORY;
}


const istd::CString& CFolderMonitorComp::GetPath() const
{
	return m_currentFolderPath;
}


void CFolderMonitorComp::SetPath(const istd::CString& path)
{
	if (path != m_currentFolderPath){
		istd::CChangeNotifier notifier(this);

		SetFolderPath(path);
	}
}


// reimplemented (icomp::IComponent)

void CFolderMonitorComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_notificationFrequencyAttrPtr.IsValid()){
		m_notificationFrequency = *m_notificationFrequencyAttrPtr;
	}

	if (m_poolingFrequencyAttrPtr.IsValid()){
		m_poolingFrequency = *m_poolingFrequencyAttrPtr;
	}

	if (m_fileFilterExpressionsAttrPtr.IsValid()){
		for (int filterIndex = 0; filterIndex < m_fileFilterExpressionsAttrPtr.GetCount(); filterIndex++){
			m_fileFilterExpressions.push_back(iqt::GetQString(m_fileFilterExpressionsAttrPtr[filterIndex]));
		}
	}

	connect(&m_fileSystemWatcher, SIGNAL(directoryChanged(const QString&)), this, SLOT(OnDirectoryChanged(const QString&)));
	connect(this, SIGNAL(FolderChanged(int)), this, SLOT(OnFolderChanged(int)), Qt::QueuedConnection);
}


void CFolderMonitorComp::OnComponentDestroyed()
{
	StopObserverThread();

	BaseClass::OnComponentDestroyed();
}


// reimplemented (iser::ISerializable)

bool CFolderMonitorComp::Serialize(iser::IArchive& archive)
{	
	StopObserverThread();

	if (!archive.IsStoring()){
		ResetFiles();
	}

	static iser::CArchiveTag pathTag("Path", "File path");
	bool retVal = archive.BeginTag(pathTag);
	retVal = retVal && archive.Process(m_currentFolderPath);
	retVal = retVal && archive.EndTag(pathTag);

	static iser::CArchiveTag directorySnapShotTag("DirectorySnapshot", "List of already monitored files");
	static iser::CArchiveTag processedFileTag("MonitoredFile", "Already monitored file");

	int processedFileCount = m_directoryFiles.count();
	retVal = retVal && archive.BeginMultiTag(directorySnapShotTag, processedFileTag, processedFileCount);
	if (archive.IsStoring()){
		for (int fileIndex = 0; fileIndex < processedFileCount; fileIndex++){
			istd::CString filePath = iqt::GetCString(m_directoryFiles[fileIndex].absoluteFilePath());
			retVal = retVal && archive.BeginTag(processedFileTag);
			retVal = retVal && archive.Process(filePath);
			retVal = retVal && archive.EndTag(processedFileTag);		
		}
	}
	else{
		for (int fileIndex = 0; fileIndex < processedFileCount; fileIndex++){
			istd::CString filePath;
			retVal = retVal && archive.BeginTag(processedFileTag);
			retVal = retVal && archive.Process(filePath);
			retVal = retVal && archive.EndTag(processedFileTag);

			if (retVal){
				m_directoryFiles.push_back(QFileInfo(iqt::GetQString(filePath)));
			}
		}	
	}

	retVal = retVal && archive.EndTag(directorySnapShotTag);

	StartObserverThread();
	
	return retVal;
}


// protected methods

// reimplemented (QThread)

void CFolderMonitorComp::run()
{
	iqt::CTimer fullUpdateTimer;
	iqt::CTimer directoryChangesUpdateTimer;
	
	I_ASSERT(!m_currentFolderPath.IsEmpty());

	while (!m_finishThread){
		QDir folderDir(iqt::GetQString(m_currentFolderPath));

		bool needFullDirectoryUpdate = (fullUpdateTimer.GetElapsed() >= m_poolingFrequency);
		bool needDirectoryChangesUpdate = (directoryChangesUpdateTimer.GetElapsed() >= m_notificationFrequency);
		bool needUpdate = ((needDirectoryChangesUpdate && !m_directoryChangesConfirmed)  || needFullDirectoryUpdate);
		if (!needUpdate){
			msleep(50);

			continue;
		}

		QDir::Filters itemFilter = QDir::AllEntries;
		if (m_observingItemsAttrPtr.IsValid()){
			itemFilter = QDir::Filters(*m_observingItemsAttrPtr);
		}

		QFileInfoList currentFiles = folderDir.entryInfoList(m_fileFilterExpressions, itemFilter | QDir::NoDotAndDotDot);

		QStringList addedFiles;
		QStringList removedFiles;
		QStringList modifiedFiles;
		QStringList attributeChangedFiles;

		int observingFlags = iabc::IDirectoryMonitorParams::OC_ALL;

		if (m_observingChangesAttrPtr.IsValid()){
			observingFlags = *m_observingChangesAttrPtr;
		}

		// check for changes:

		if ((observingFlags & iabc::IDirectoryMonitorParams::OC_REMOVE) != 0){
			for (int fileIndex = 0; fileIndex < int(m_directoryFiles.count()); fileIndex++){
				QFileInfo fileInfo = m_directoryFiles[fileIndex];
				if (!fileInfo.exists()){
					removedFiles.push_back(fileInfo.absoluteFilePath());
					SendInfoMessage(0, iqt::GetCString(fileInfo.absoluteFilePath() + " was removed"));
				}
			}
		}

		if (observingFlags != iabc::IDirectoryMonitorParams::OC_REMOVE){
			for (int currentFileIndex = 0; currentFileIndex < int(currentFiles.count()); currentFileIndex++){
				const QFileInfo& currentFileInfo = currentFiles[currentFileIndex];

				QFileInfoList::iterator foundFileIter = qFind(m_directoryFiles.begin(), m_directoryFiles.end(), currentFileInfo);
				if (foundFileIter == m_directoryFiles.end()){
					if ((observingFlags & iabc::IDirectoryMonitorParams::OC_ADD) != 0){
						addedFiles.push_back(currentFileInfo.absoluteFilePath());
						SendInfoMessage(0, iqt::GetCString(currentFileInfo.absoluteFilePath() + " was added"));
					}
				}
				else{
					if (foundFileIter->lastModified() != currentFileInfo.lastModified()){
						if ((observingFlags & iabc::IDirectoryMonitorParams::OC_MODIFIED) != 0){
							modifiedFiles.push_back(currentFileInfo.absoluteFilePath());
							SendInfoMessage(0, iqt::GetCString(currentFileInfo.absoluteFilePath() + " was modified"));
						}
					}
					if (foundFileIter->permissions() != currentFileInfo.permissions()){
						if ((observingFlags & iabc::IDirectoryMonitorParams::OC_ATTR_CHANGED) != 0){
							attributeChangedFiles.push_back(currentFileInfo.absoluteFilePath());
							SendInfoMessage(0, istd::CString("Attributes of") + iqt::GetCString(currentFileInfo.absoluteFilePath() + " have been changed"));
						}
					}
				}
			}
		}

		m_directoryFiles = currentFiles;

		isys::CSectionBlocker block(&m_lock);

		m_folderChanges.addedFiles = addedFiles;
		m_folderChanges.removedFiles = removedFiles;
		m_folderChanges.modifiedFiles = modifiedFiles;
		m_folderChanges.attributeChangedFiles = attributeChangedFiles;

		int changeFlags = CF_MODEL;

		if (!addedFiles.isEmpty()){
			changeFlags |= FilesAdded;
		}

		if (!removedFiles.isEmpty()){
			changeFlags |= FilesRemoved;
		}

		if (!modifiedFiles.isEmpty()){
			changeFlags |= FilesModified;
		}

		if (!attributeChangedFiles.isEmpty()){
			changeFlags |= FilesAttributeChanged;
		}

		Q_EMIT FolderChanged(changeFlags);

		m_directoryChangesConfirmed = true;
		
		fullUpdateTimer.Start();
		directoryChangesUpdateTimer.Start();
	}
}


// private slots

void CFolderMonitorComp::OnDirectoryChanged(const QString&/* directoryPath*/)
{
	m_directoryChangesConfirmed = false;
}


void CFolderMonitorComp::OnFolderChanged(int changeFlags)
{
	istd::CChangeNotifier changePtr(this, changeFlags, &m_folderChanges);

	changePtr.Reset();
}


// private methods

void CFolderMonitorComp::SetFolderPath(const istd::CString& folderPath)
{	
	if (folderPath != m_currentFolderPath){
		SendInfoMessage(0, istd::CString("Start observing of: ") + folderPath, "FolderMonitor");

		if (!m_currentFolderPath.IsEmpty()){
			m_fileSystemWatcher.removePath(iqt::GetQString(m_currentFolderPath));
		}

		StopObserverThread();

		ResetFiles();

		m_currentFolderPath = folderPath;


		QFileInfo fileInfo(iqt::GetQString(m_currentFolderPath));
		if (fileInfo.exists()){
			QDir folderDir(iqt::GetQString(m_currentFolderPath));	
			
			QDir::Filters itemFilter = QDir::AllEntries;
			if (m_observingItemsAttrPtr.IsValid()){
				itemFilter = QDir::Filters(*m_observingItemsAttrPtr);
			}

			m_directoryFiles = folderDir.entryInfoList(m_fileFilterExpressions, itemFilter | QDir::NoDotAndDotDot);

			m_fileSystemWatcher.addPath(iqt::GetQString(m_currentFolderPath));

			StartObserverThread();
		}
		else{
			SendWarningMessage(0, istd::CString("Directory: ") + folderPath + istd::CString(" not exists. Observing aborted"), "FolderMonitor");
		}
	}
}


void CFolderMonitorComp::StartObserverThread()
{
	m_finishThread = false;

	BaseClass2::start();
}


void CFolderMonitorComp::StopObserverThread()
{
	m_finishThread = true;

	// wait for 30 seconds for finishing of thread: 
	iqt::CTimer timer;
	while (timer.GetElapsed() < 30 && BaseClass2::isRunning()){
		QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
	}

	if (BaseClass2::isRunning()){
		BaseClass2::terminate();
	}
}


void CFolderMonitorComp::ResetFiles()
{
	m_directoryFiles.clear();
	m_folderChanges.addedFiles.clear();
	m_folderChanges.attributeChangedFiles.clear();
	m_folderChanges.modifiedFiles.clear();
	m_folderChanges.removedFiles.clear();
}


} // namespace iabc


