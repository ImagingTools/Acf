#include "icomp/CCompositeComponent.h"


#include "icomp/IRegistry.h"


namespace icomp
{


CCompositeComponent::CCompositeComponent(const CCompositeComponentContext* contextPtr)
:	m_context(*contextPtr)
{
	I_ASSERT(contextPtr != NULL);
}


// reimplemented (icomp::IComponent)

void* CCompositeComponent::GetInterface(const type_info& interfaceType, const ::std::string& subId)
{
	const IRegistry& registry = m_context.GetRegistry();

	if (subId.empty()){
		const IRegistry::InterfaceInfos& interfaceInfos = registry.GetExportedInterfaceInfos();
		IRegistry::InterfaceInfos::const_iterator iter = interfaceInfos.find(interfaceType.name());
		if (iter != interfaceInfos.end()){
			const IRegistry::InterfaceInfo& info = iter->second;
			IComponent* componentPtr = m_context.GetSubcomponent(info.componentId);
			if (componentPtr != NULL){
				return componentPtr->GetInterface(interfaceType, info.subId);
			}
		}
	}
	else{
		::std::string componentId;
		::std::string restId;
		::std::string::size_type pointPos = subId.find('.');
		if (pointPos != ::std::string::npos){
			componentId = componentId.substr(0, pointPos);
			restId = componentId.substr(pointPos + 1);
		}
		else{
			componentId = componentId;
		}

		const IRegistry::SubcomponentMap& subcomponentMap = registry.GetExportedSubcomponentMap();
		IRegistry::SubcomponentMap::const_iterator iter = subcomponentMap.find(componentId);
		if (iter != subcomponentMap.end()){
			const ::std::string& realComponentId = iter->second;
			IComponent* componentPtr = m_context.GetSubcomponent(realComponentId);
			if (componentPtr != NULL){
				return componentPtr->GetInterface(interfaceType, restId);
			}
		}
	}

	return NULL;
}


}//namespace icomp


