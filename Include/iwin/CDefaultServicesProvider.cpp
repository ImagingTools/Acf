#include "iwin/CDefaultServicesProvider.h"


#include "istd/TSingleFactory.h"
#include "istd/CStaticServicesProvider.h"

#include "iwin/CTimer.h"
#include "iwin/CApplicationEnvironment.h"
#include "iwin/CCriticalSection.h"
#include "iwin/CSystemEnvironment.h"


namespace iwin
{


void CDefaultServicesProvider::RegisterServices()
{
	static iwin::CApplicationEnvironment applicationEnvironment;
	istd::CStaticServicesProvider::RegisterService<isys::IApplicationEnvironment>(&applicationEnvironment);

	static iwin::CCriticalSection criticalSection;
	istd::CStaticServicesProvider::RegisterService<isys::ICriticalSection>(&criticalSection);

	static iwin::CSystemEnvironment systemEnvironment;
	istd::CStaticServicesProvider::RegisterService<isys::ISystemEnvironment>(&systemEnvironment);

	static istd::TSingleFactory<isys::ITimer, iwin::CTimer> timerFactory("");
	istd::CStaticServicesProvider::RegisterFactory(&timerFactory);
}


} // namespace iwin


