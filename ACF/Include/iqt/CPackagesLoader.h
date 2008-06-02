#ifndef iqt_CPackagesLoader_included
#define iqt_CPackagesLoader_included


// Qt includes
#include <QFileInfo>

#include "istd/TDelPtr.h"

#include "icomp/CPackageStaticInfo.h"

#include "iqt/CDllFunctionsProvider.h"


namespace iqt
{


/**
	Loads component packages from dynamic link libraries.
*/
class CPackagesLoader: public icomp::CPackageStaticInfo
{
public:
	bool RegisterPackageFile(const istd::CString& file, bool beQuiet = true);
	bool RegisterPackagesDir(const istd::CString& subDir, bool beQuiet = true);
	bool LoadConfigFile(const istd::CString& configFile);

protected:
	CDllFunctionsProvider& GetProviderRef(const QFileInfo& fileInfo, bool beQuiet = true);

private:
	typedef istd::TDelPtr<CDllFunctionsProvider> FunctionsProviderPtr;
	typedef std::map<QString, FunctionsProviderPtr> DllCacheMap;

	DllCacheMap m_dllCacheMap;
};


} // namespace iqt


#endif // !iqt_CPackagesLoader_included


