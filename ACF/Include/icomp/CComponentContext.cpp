#include "icomp/CComponentContext.h"


#include "icomp/IRegistryElement.h"


namespace icomp
{


CComponentContext::CComponentContext(const IRegistryElement* elementPtr, const IComponentContext* parentPtr)
:	m_registryElement(*elementPtr), m_parentPtr(parentPtr)
{
}


// reimplemented (icomp::IComponentContext)

const IRegistryElement& CComponentContext::GetRegistryElement() const
{
	return m_registryElement;
}


const IComponentContext* CComponentContext::GetParentContext() const
{
	return m_parentPtr;
}


const iser::ISerializable* CComponentContext::GetAttribute(const ::std::string& attributeId, const IComponentContext** realContextPtr) const
{
	const IRegistryElement::AttributeInfo* infoPtr = m_registryElement.GetAttributeInfo(attributeId);

	if (infoPtr  != NULL){
		const ::std::string& exportId = infoPtr->exportId;
		if (!exportId.empty() && (m_parentPtr != NULL)){
			const iser::ISerializable* parentAttributePtr = m_parentPtr->GetAttribute(exportId, realContextPtr);

			if (parentAttributePtr != NULL){
				return parentAttributePtr;
			}
		}

		if (infoPtr->attributePtr.IsValid()){
			if (realContextPtr != NULL){
				*realContextPtr = this;
			}

			return infoPtr->attributePtr.GetPtr();
		}
	}

	return NULL;
}


IComponent* CComponentContext::GetSubcomponent(const ::std::string& /*componentId*/) const
{
	return NULL;	// normal component has no subcomponents
}


}//namespace icomp


