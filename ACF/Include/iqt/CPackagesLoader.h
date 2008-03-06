#ifndef iqt_CPackagesLoader_included
#define iqt_CPackagesLoader_included


#include "icomp/CPackageStaticInfo.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	Loads packages from dynamic link libraries.
*/
class CPackagesLoader: public icomp::CPackageStaticInfo
{
public:
	bool RegisterPackageFile(const istd::CString& file, bool beQuiet = true);
	bool RegisterPackagesDir(const istd::CString& directory, bool beQuiet = true);
	bool LoadConfigFile(const istd::CString& configFile);
};


} // namespace iqt


#endif // !iqt_CPackagesLoader_included


