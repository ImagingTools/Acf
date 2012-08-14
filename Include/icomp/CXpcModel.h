#ifndef icomp_CXpcModel_included
#define icomp_CXpcModel_included


// Qt includes
#include <QtCore/QStringList>

// ACF includes
#include "iser/ISerializable.h"


namespace icomp
{


/**
	Data model for ACF package loader configuration.
*/
class CXpcModel: virtual public iser::ISerializable
{
public:
	// gui interface (for editing)
	const QStringList& GetConfFilesList() const;
	void SetConfFilesList(const QStringList& list);
	const QStringList& GetPackageDirsList() const;
	void SetPackageDirsList(const QStringList& list);
	const QStringList& GetPackagesList() const;
	void SetPackagesList(const QStringList& list);

	// program interface (for loading XPC files)
	virtual int GetNumConfFiles() const;
	virtual int GetNumPackageDirs() const;
	virtual int GetNumPackages() const;
	virtual QString GetConfFile(int) const;
	virtual QString GetPackageDir(int) const;
	virtual QString GetPackage(int) const;
	virtual void AddConfFile(const QString& path);
	virtual void AddPackageDir(const QString& path);
	virtual void AddPackage(const QString& path);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	QStringList m_confFiles;
	QStringList m_packageDirs;
	QStringList m_packages;
};


} // namespace icomp


#endif // !icomp_CXpcModel_included


