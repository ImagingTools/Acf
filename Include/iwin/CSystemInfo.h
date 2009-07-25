#ifndef iwin_CSystemInfo_included
#define iwin_CSystemInfo_included


#include "isys/CSystemInfoBase.h"

#include "iwin/iwin.h"


namespace iwin
{


/**
	A system info implementation for the Windows plattform

	\ingroup Windows
*/
class CSystemInfo: public isys::CSystemInfoBase
{
public:
	CSystemInfo();

protected:
	void GetSystemInfo();

private:
	static NetworkAdapters GetNetworkAdapters();
};


} // namespace iwin


#endif // !iwin_CSystemInfo_included


