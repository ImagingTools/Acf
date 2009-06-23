#ifndef iabc_CFolderMonitorComp_included
#define iabc_CFolderMonitorComp_included


#include "iabc/iabc.h"


// Qt includes
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QTimer>
#include <QThread>


// ACF includes
#include "isys/IFolderMonitor.h"

#include "icomp/CComponentBase.h"

#include "ibase/TLoggerCompWrap.h"

#include "iqt/CCriticalSection.h"


namespace iabc
{


/**
	Component for monitoring of the file folder.
*/
class CFolderMonitorComp:
			public QThread,
			public ibase::TLoggerCompWrap<icomp::CComponentBase>,
			virtual public isys::IFolderMonitor
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

	typedef ibase::TLoggerCompWrap<icomp::CComponentBase> BaseClass;
	typedef QThread BaseClass2;

	I_BEGIN_COMPONENT(CFolderMonitorComp);
		I_REGISTER_INTERFACE(isys::IFolderMonitor);
		I_ASSIGN(m_notificationFrequencyAttrPtr, "NotificationFrequency", "Minimal time range for the folder check after the change notification", false, 10);
		I_ASSIGN(m_poolingFrequencyAttrPtr, "PoolingFrequency", "Minimal frequency for pooling of changes in seconds", false, 60);
		I_ASSIGN_MULTI_0(m_fileFilterExpressionsAttrPtr, "FileFilters", "File filters for the folder (as regular expression)", false);
		I_ASSIGN(m_defaultPathAttrPtr, "DefaultPath", "Default folder to observe", false, "DefaultPath");
	I_END_COMPONENT;

	CFolderMonitorComp();

	// reimplemented (isys::IFolderMonitor)
	virtual void SetFolderPath(const istd::CString& folderPah);
	virtual istd::CString GetFolderPath() const;
	virtual istd::CStringList GetChangedFileItems(int changeFlags) const;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected:
	// reimplemented (QThread)
	virtual void run();

private slots:
	void OnDirectoryChanged(const QString& directoryPath);
	void OnFolderChanged(int changeFlags);

Q_SIGNALS:
	void FolderChanged(int changeFlags);

private:
	I_ATTR(int, m_notificationFrequencyAttrPtr);
	I_ATTR(int, m_poolingFrequencyAttrPtr);
	I_ATTR(istd::CString, m_defaultPathAttrPtr);
	I_MULTIATTR(istd::CString, m_fileFilterExpressionsAttrPtr)

	QFileSystemWatcher m_fileSystemWatcher;

	QFileInfoList m_directoryFiles;
	QString m_folderPath;

	iqt::CCriticalSection m_lock;

	bool m_finishThread;
	bool m_directoryChangesConfirmed;

	// Attribute shadows:
	int m_notificationFrequency;
	int m_poolingFrequency;
	QStringList m_fileFilterExpressions;

	FileSystemChanges m_folderChanges;
};


} // namespace iabc


#endif // !iabc_CFolderMonitorComp_included


