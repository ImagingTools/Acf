#include "iqt/CDefaultServicesProvider.h"

#include "istd/CStaticServicesProvider.h"


namespace iqt
{


void CDefaultServicesProvider::RegisterServices()
{
	istd::CStaticServicesProvider::RegisterService<isys::ITimer>(&s_timer);
	istd::CStaticServicesProvider::RegisterService<isys::IApplicationEnvironment>(&s_applicationEnvironment);
}


iqt::CTimer CDefaultServicesProvider::s_timer;
iqt::CApplicationEnvironment CDefaultServicesProvider::s_applicationEnvironment;


} // namespace iqt


