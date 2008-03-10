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
		const IRegistry::ExportedInterfacesMap& interfaceInfos = registry.GetExportedExportedInterfacesMap();
		IRegistry::ExportedInterfacesMap::const_iterator iter = interfaceInfos.find(interfaceType.name());
		if (iter != interfaceInfos.end()){
			::std::string componentId;
			::std::string restId;
			SplitComponentId(iter->second, componentId, restId);

			IComponent* componentPtr = m_context.GetSubcomponent(componentId);
			if (componentPtr != NULL){
				return componentPtr->GetInterface(interfaceType, restId);
			}
		}
	}
	else{
		::std::string componentId;
		::std::string restId;
		SplitComponentId(subId, componentId, restId);

		const IRegistry::ExportedComponentsMap& subcomponentMap = registry.GetExportedComponentsMap();
		IRegistry::ExportedComponentsMap::const_iterator iter = subcomponentMap.find(componentId);
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


// protected methods

// static methods

void CCompositeComponent::SplitComponentId(const ::std::string& fullId, ::std::string& componentId, ::std::string& restId)
{
	::std::string::size_type pointPos = fullId.find('.');
	if (pointPos != ::std::string::npos){
		componentId = fullId.substr(0, pointPos);
		restId = componentId.substr(pointPos + 1);
	}
	else{
		componentId = fullId;
		restId = "";
	}
}


}//namespace icomp


