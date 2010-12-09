#ifndef icomp_IRegistryLoader_included
#define icomp_IRegistryLoader_included


#include "istd/IPolymorphic.h"
#include "istd/CString.h"


namespace icomp
{


class IRegistry;


/**
	Interface for a ACF registry loader.
*/
class IRegistryLoader: virtual public istd::IPolymorphic
{
public:
	/**
		Get an ACF registry from a given file.
	*/
	virtual const icomp::IRegistry* GetRegistryFromFile(const istd::CString& path) const = 0;
};
	

} // namespace icomp


#endif // !icomp_IRegistryLoader_included


