#include "icomp/CCompositeComponentContext.h"


#include "icomp/IRegistry.h"
#include "icomp/IRegistriesManager.h"


namespace icomp
{


CCompositeComponentContext::CCompositeComponentContext(
			const IRegistryElement* elementPtr,
			const IComponentStaticInfo* staticInfoPtr,
			const IRegistry* registryPtr,
			const IComponentEnvironmentManager* environmentManagerPtr,
			const CCompositeComponentContext* parentPtr)
:	BaseClass(elementPtr, staticInfoPtr, parentPtr),
	m_registry(*registryPtr),
	m_environmentManager(*environmentManagerPtr)
{
	I_ASSERT(registryPtr != NULL);
	I_ASSERT(environmentManagerPtr != NULL);
}


}//namespace icomp


