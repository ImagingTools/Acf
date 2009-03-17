#ifndef iqt_CDefaultServicesProvider_included
#define iqt_CDefaultServicesProvider_included


#include "iqt/CTimer.h"
#include "iqt/CApplicationEnvironment.h"
#include "iqt/CCriticalSection.h"
#include "iqt/CFileSystem.h"
#include "iqt/CDefaultRegistryLoaderProvider.h"


namespace iqt
{


/**
	Default services provider for qt based services
 */
class CDefaultServicesProvider
{
public:
	static void RegisterServices();

private:
	static iqt::CTimer s_timer;
	static iqt::CApplicationEnvironment s_applicationEnvironment;
	static iqt::CCriticalSection s_criticalSection;
	static iqt::CFileSystem s_fileSystem;
	static iqt::CDefaultRegistryLoaderProvider s_registryLoaderProvider;
};


} // namespace iqt


#endif // !iqt_CDefaultServicesProvider_included


