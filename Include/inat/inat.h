#ifndef inat_included
#define inat_included


#ifdef _WIN32
#include "iwin/iwin.h"
#else
#include "iqt/iqt.h"
#endif


/**
	This namespace contains implementations for the compile-time native plattform
*/
namespace inat
{


#ifdef _WIN32
	using namespace iwin;
#else
	using namespace iqt;
#endif


} // namespace inat


#endif // !inat_included
