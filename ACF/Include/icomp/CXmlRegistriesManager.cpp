#include "icomp/CXmlRegistriesManager.h"


#include "iser/CXmlFileReadArchive.h"


namespace icomp
{


iser::IArchive* CXmlRegistriesManager::CreateArchive(const istd::CString& path) const
{
	return new iser::CXmlFileReadArchive(path);
}


}//namespace icomp


