#ifndef icomp_IRegistryLoaderProvider_included
#define icomp_IRegistryLoaderProvider_included


#include "icomp/IRegistryLoader.h"


namespace icomp
{

/**
	Interface for a ACF registry loader provider.
*/
class IRegistryLoaderProvider: virtual public istd::IPolymorphic
{
public:
	virtual icomp::IRegistryLoader* GetRegistryLoader() const = 0;

};
	

} // namespace icomp


#endif // !icomp_IRegistryLoaderProvider_included


