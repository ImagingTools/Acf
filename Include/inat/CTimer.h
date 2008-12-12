#ifndef inat_CTimer_included
#define inat_CTimer_included


#include "inat/inat.h"


#ifdef _WIN32
#include "iwin/CTimer.h"
#elif defined QT_CORE_LIB
#include "iqt/CTimer.h"
#elif
#error Operating system not supported
#endif


#endif // !inat_CTimer_included


