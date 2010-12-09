#ifndef icomp_IRegistryLoaderProvider_included
#define icomp_IRegistryLoaderProvider_included


#include "istd/IPolymorphic.h"


namespace icomp
{


class IRegistryLoader;
class IComponentEnvironmentManager;


/**
	Interface for a ACF registry loader provider.
*/
class IRegistryLoaderProvider: virtual public istd::IPolymorphic
{
public:
	virtual IComponentEnvironmentManager* GetEnvironmentManager() const = 0;
	virtual IRegistryLoader* GetRegistryLoader() const = 0;
};
	

} // namespace icomp


#endif // !icomp_IRegistryLoaderProvider_included


