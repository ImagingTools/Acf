#ifndef iabc_CFolderMonitorComp_included
#define iabc_CFolderMonitorComp_included


// Qt includes
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QTimer>
#include <QThread>
#include <QDir>


// ACF includes
#include "imod/CSingleModelObserverBase.h"

#include "icomp/CComponentBase.h"

#include "ibase/TLoggerCompWrap.h"

#include "iprm/IFileNameParam.h"

#include "iqt/CCriticalSection.h"

#include "iabc/IDirectoryMonitor.h"
#include "iabc/IDirectoryMonitorParams.h"


namespace iabc
{


/**
	Component for monitoring of the file folder.
*/
class CFolderMonitorComp:
			public QThread,
			public ibase::CLoggerComponentBase,
			virtual public iabc::IDirectoryMonitor,
			virtual public iprm::IFileNameParam
{
	Q_OBJECT

public:
	class FileSystemChanges: public istd::IPolymorphic
	{
	public:
		QStringList addedFiles;
		QStringList removedFiles;
		QStringList modifiedFiles;
		QStringList attributeChangedFiles;
	};

	typedef ibase::CLoggerComponentBase BaseClass;
	typedef QThread BaseClass2;

	I_BEGIN_COMPONENT(CFolderMonitorComp);
		I_REGISTER_INTERFACE(iabc::IDirectoryMonitor);
		I_REGISTER_INTERFACE(ibase::IFileListProvider);
		I_REGISTER_INTERFACE(iprm::IFileNameParam);
		I_ASSIGN(m_notificationFrequencyAttrPtr, "NotificationFrequency", "Minimal time range for the folder check after the change notification", false, 10);
		I_ASSIGN(m_poolingFrequencyAttrPtr, "PoolingFrequency", "Minimal frequency for pooling of changes in seconds", false, 60);
		I_ASSIGN(m_observingItemsAttrPtr, "ObserveItems", "Select the item types to be observed.\n1 - Directories\n2 - Files\n4 - Drives", false, iabc::IDirectoryMonitorParams::OI_ALL);
		I_ASSIGN(m_observingChangesAttrPtr, "ObserveChanges", "Select change types to be observed.\n1 - Added\n2 - Removed\n4 - Modified\n8 - Changes in attributes", false, iabc::IDirectoryMonitorParams::OI_ALL);
		I_ASSIGN_MULTI_0(m_fileFilterExpressionsAttrPtr, "FileFilters", "File filters for the folder (as regular expression)", false);
	I_END_COMPONENT;

	CFolderMonitorComp();

	// reimplemented (iabc::IDirectoryMonitor)
	virtual istd::CStringList GetChangedFileItems(int changeFlags) const;

	// reimplemented (ibase::IFileListProvider)
	virtual istd::CStringList GetFileList() const;

	// reimplemented (iprm::IFileNameParam)
	virtual int GetPathType() const;
	virtual const istd::CString& GetPath() const;
	virtual void SetPath(const istd::CString& path);

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	// reimplemented (QThread)
	virtual void run();

private slots:
	/**
		Slot is triggered from QFileSystemWatcher, if the folder has benn changed.
	*/
	void OnDirectoryChanged(const QString& directoryPath);
	
	/**
		Delegate folder change event via istd::CChangeNotifier from main thread.
	*/
	void OnFolderChanged(int changeFlags);

Q_SIGNALS:
	/**
		Signal is emitted from observing thread to notifiy the main thread about changes in a directory.
	*/
	void FolderChanged(int changeFlags);

private:
	void SetFolderPath(const istd::CString& folderPath);
	void StartObserverThread();
	void StopObserverThread();
	void ResetFiles();

private:
	I_ATTR(int, m_notificationFrequencyAttrPtr);
	I_ATTR(int, m_poolingFrequencyAttrPtr);
	I_ATTR(int, m_observingItemsAttrPtr);
	I_ATTR(int, m_observingChangesAttrPtr);
	I_ATTR(istd::CString, m_defaultPathAttrPtr);
	I_MULTIATTR(istd::CString, m_fileFilterExpressionsAttrPtr)
	I_REF(iprm::IFileNameParam, m_fileNameParamCompPtr);

	QFileSystemWatcher m_fileSystemWatcher;

	QFileInfoList m_directoryFiles;

	mutable iqt::CCriticalSection m_lock;

	bool m_finishThread;
	bool m_directoryChangesConfirmed;

	istd::CString m_currentFolderPath;

	FileSystemChanges m_folderChanges;

	// Attribute shadows:
	int m_notificationFrequency;
	int m_poolingFrequency;
	QStringList m_fileFilterExpressions;
};


} // namespace iabc


#endif // !iabc_CFolderMonitorComp_included


