#include "icomp/CBaseComponentStaticInfo.h"


namespace icomp
{


CBaseComponentStaticInfo::CBaseComponentStaticInfo(const IComponentStaticInfo* baseComponentPtr)
:	m_baseComponentPtr(baseComponentPtr)
{
	if (baseComponentPtr != NULL){
		m_interfaceExtractors.SetParent(&baseComponentPtr->GetInterfaceExtractors());
		m_attributeInfos.SetParent(&baseComponentPtr->GetAttributeInfos());
	}
}


//	reimplemented (icomp::IComponentStaticInfo)

IComponent* CBaseComponentStaticInfo::CreateComponent() const
{
	I_CRITICAL();	// trying to create abstract base component.

	return NULL;
}


const IComponentStaticInfo::InterfaceExtractors& CBaseComponentStaticInfo::GetInterfaceExtractors() const
{
	return m_interfaceExtractors;
}


const istd::CClassInfo* CBaseComponentStaticInfo::FindInterfaceInfo(const std::string& interfaceId) const
{
	int interfacesCount = m_interfaceExtractors.GetElementsCount();
	for (int i = 0; i < interfacesCount; ++i){
		const istd::CClassInfo& interfaceInfo = m_interfaceExtractors.GetKeyAt(i);

		if (interfaceInfo.GetName() == interfaceId){
			return &interfaceInfo;
		}
	}

	return BaseClass::FindInterfaceInfo(interfaceId);
}


const IComponentStaticInfo::AttributeInfos& CBaseComponentStaticInfo::GetAttributeInfos() const
{
	return m_attributeInfos;
}


IComponentStaticInfo::Ids CBaseComponentStaticInfo::GetSubcomponentIds() const
{
	Ids retVal = BaseClass::GetSubcomponentIds();

	if (m_baseComponentPtr != NULL){
		Ids baseIds = m_baseComponentPtr->GetSubcomponentIds();

		retVal.insert(baseIds.begin(), baseIds.end());
	}

	return retVal;
}


const IComponentStaticInfo* CBaseComponentStaticInfo::GetSubcomponentInfo(const std::string& subcomponentId) const
{
	const IComponentStaticInfo* retVal = BaseClass::GetSubcomponentInfo(subcomponentId);
	if (retVal != NULL){
		return retVal;
	}
	else if (m_baseComponentPtr != NULL){
		return m_baseComponentPtr->GetSubcomponentInfo(subcomponentId);
	}
	else{
		return NULL;
	}
}


bool CBaseComponentStaticInfo::RegisterInterfaceExtractor(const istd::CClassInfo& interfaceId, InterfaceExtractorPtr extractorPtr)
{
	return m_interfaceExtractors.InsertLocal(interfaceId, extractorPtr);
}


bool CBaseComponentStaticInfo::RegisterAttributeInfo(const std::string& attributeId, const IAttributeStaticInfo* attributeInfoPtr)
{
	return m_attributeInfos.InsertLocal(attributeId, attributeInfoPtr);
}


}//namespace icomp


