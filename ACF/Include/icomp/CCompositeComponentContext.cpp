#include "icomp/CCompositeComponentContext.h"


#include "icomp/IComponentStaticInfo.h"
#include "icomp/IRegistryElement.h"


namespace icomp
{


CCompositeComponentContext::CCompositeComponentContext(
			const IRegistryElement* elementPtr,
			const IRegistry* registryPtr,
			const CCompositeComponentContext* parentPtr)
:	BaseClass(elementPtr, parentPtr)
{
	// TODO: implement CCompositeComponentContext!!!
}


// reimplemented (icomp::IComponentContext)

IComponent* CCompositeComponentContext::GetSubComponent(const ::std::string& componentId) const
{
	ComponentMap::iterator iter = m_componentMap.find(componentId);
	if (iter == m_componentMap.end()){
		ComponentPtr& componentPtr = m_componentMap[componentId];

		const IRegistryElement& registryElement = GetRegistryElement();
		const IComponentStaticInfo& staticInfo = registryElement.GetComponentStaticInfo();
		const IComponentStaticInfo::SubcomponentInfos& subcomponentInfos = staticInfo.GetSubcomponentInfos();

		// TODO: implement it correct!
		int subcomponentIndex = subcomponentInfos.FindIndex(componentId);

		if (subcomponentIndex >= 0){
			const IComponentStaticInfo* subStaticInfoPtr = subcomponentInfos.GetValueAt(subcomponentIndex);

			if (subStaticInfoPtr != NULL){
//				subStaticInfoPtr->CreateComponent(
			}
		}

		return componentPtr.GetPtr();
	}

	return iter->second.GetPtr();
}


}//namespace icomp


