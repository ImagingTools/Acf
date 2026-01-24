// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icomp/CCompositeComponentContext.h>


#include <icomp/IRegistry.h>
#include <icomp/IRegistriesManager.h>


namespace icomp
{


CCompositeComponentContext::CCompositeComponentContext(
			const IRegistryElement* elementPtr,
			const IComponentStaticInfo* staticInfoPtr,
			const IRegistry* registryPtr,
			const IComponentEnvironmentManager* environmentManagerPtr,
			const CCompositeComponentContext* parentPtr,
			const QByteArray& contextId)
:	BaseClass(elementPtr, staticInfoPtr, parentPtr, contextId),
	m_registry(*registryPtr),
	m_environmentManager(*environmentManagerPtr)
{
	Q_ASSERT(registryPtr != NULL);
	Q_ASSERT(environmentManagerPtr != NULL);
}


} // namespace icomp


