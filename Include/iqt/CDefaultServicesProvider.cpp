#include "iqt/CDefaultServicesProvider.h"

#include "istd/CStaticServicesProvider.h"


namespace iqt
{


void CDefaultServicesProvider::RegisterServices()
{
	istd::CStaticServicesProvider::RegisterService<isys::ITimer>(&s_timer);
	istd::CStaticServicesProvider::RegisterService<isys::IApplicationEnvironment>(&s_applicationEnvironment);
	istd::CStaticServicesProvider::RegisterService<isys::ICriticalSection>(&s_criticalSection);
	istd::CStaticServicesProvider::RegisterService<icomp::IRegistryLoaderProvider>(&s_registryLoaderProvider);
}


iqt::CTimer CDefaultServicesProvider::s_timer;
iqt::CApplicationEnvironment CDefaultServicesProvider::s_applicationEnvironment;
iqt::CCriticalSection CDefaultServicesProvider::s_criticalSection;
iqt::CDefaultRegistryLoaderProvider CDefaultServicesProvider::s_registryLoaderProvider;


} // namespace iqt


