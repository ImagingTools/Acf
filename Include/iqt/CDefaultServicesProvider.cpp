#include "iqt/CDefaultServicesProvider.h"


// ACF includes
#include "istd/TSingleFactory.h"
#include "istd/CStaticServicesProvider.h"

#include "iqt/CDefaultRegistryLoaderProvider.h"
#include "iqt/CLocalizer.h"
#include "iqt/CTrace.h"


namespace iqt
{


void CDefaultServicesProvider::RegisterServices()
{
	static iqt::CTrace trace;
	istd::CStaticServicesProvider::RegisterService<istd::ITrace>(&trace);

	static iqt::CLocalizer localizer;
	istd::CStaticServicesProvider::RegisterService<istd::ILocalizer>(&localizer);
}


} // namespace iqt


