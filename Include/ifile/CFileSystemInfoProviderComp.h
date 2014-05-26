#ifndef ifile_CFileSystemInfoProviderComp_included
#define ifile_CFileSystemInfoProviderComp_included


// Qt includes
#include <QThread>
#include <QMutex>
#include <QVector>

// ACF includes
#include "ilog/TLoggerCompWrap.h"
#include "ifile/IFileSystemInfoProvider.h"


namespace ifile
{


class CFileSystemInfoProviderComp:
			protected QThread,
			public ilog::CLoggerComponentBase, 
			virtual public ifile::IFileSystemInfoProvider,
			virtual public iprm::IOptionsList
{
	Q_OBJECT
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CFileSystemInfoProviderComp);
		I_REGISTER_INTERFACE(ifile::IFileSystemInfoProvider);
		I_REGISTER_INTERFACE(iprm::IOptionsList);
		I_ASSIGN(m_autoUpdatePeriodAttr, "AutomaticUpdatePeriod", "Automatic updates period, s", false, 10);
	I_END_COMPONENT;

	CFileSystemInfoProviderComp();

	// reimplemented (ifile::IFileSystemInfoProvider)
	virtual const iprm::IOptionsList& GetDriveList() const;
	virtual const istd::CSystem::FileDriveInfo* GetFileDriveInfo(int driveIndex = -1) const;

	// reimplemented (iprm::IOptionsList)
	virtual int GetOptionsFlags() const;
	virtual int GetOptionsCount() const;
	virtual QString GetOptionName(int index) const;
	virtual QString GetOptionDescription(int index) const;
	virtual QByteArray GetOptionId(int index) const;
	virtual bool IsOptionEnabled(int index) const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// reimplemented (QThread)
	virtual void run();

protected:
	struct DriveInfo
	{
		istd::CSystem::FileDriveInfo info;
		QString name;
		QByteArray id;

		bool operator == (const DriveInfo& driveInfo) const
		{
			return (info == driveInfo.info) && (name == driveInfo.name) && (id == driveInfo.id);
		}
	};

	typedef QVector<DriveInfo> DriveInfos;

Q_SIGNALS:
	void EmitUpdate(const DriveInfos& driveInfos);

private Q_SLOTS:
	void OnUpdate(const DriveInfos& driveInfos);

private:
	DriveInfos CalculateDriveInfos() const;

private:
	DriveInfos m_driveInfos;

	int m_sleepInterval;
	bool m_threadTerminationRequested;

	mutable QMutex m_lock;

	I_ATTR(int, m_autoUpdatePeriodAttr);
};


} // namespace ifile


#endif // !ifile_CFileSystemInfoProviderComp_included


