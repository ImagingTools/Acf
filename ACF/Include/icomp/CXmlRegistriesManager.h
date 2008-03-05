#ifndef icomp_CXmlRegistriesManager_included
#define icomp_CXmlRegistriesManager_included


#include "icomp/CFileRegistriesManagerBase.h"


namespace icomp
{


class CXmlRegistriesManager: public CFileRegistriesManagerBase
{
protected:
	virtual iser::IArchive* CreateArchive(const istd::CString& path) const;
};


}//namespace icomp


#endif // !icomp_CXmlRegistriesManager_included


