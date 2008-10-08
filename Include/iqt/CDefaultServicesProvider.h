#ifndef iqt_CDefaultServicesProvider_included
#define iqt_CDefaultServicesProvider_included


#include "iqt/CTimer.h"
#include "iqt/CApplicationEnvironment.h"


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
};


} // namespace iqt


#endif // !iqt_CDefaultServicesProvider_included


