#include "icomp/CPackageStaticInfo.h"


namespace icomp
{


bool CPackageStaticInfo::RegisterSubcomponentInfo(const ::std::string& subcomponentId, const IComponentStaticInfo* componentInfoPtr)
{
	return m_subcomponentInfos.InsertLocal(subcomponentId, componentInfoPtr);
}


// reimplemented (icomp::IPackageStaticInfo)

IComponent* CPackageStaticInfo::CreateComponent(const IComponentContext& context) const
{
	return NULL;
}


const CPackageStaticInfo::InterfaceExtractors& CPackageStaticInfo::GetInterfaceExtractors() const
{
	static InterfaceExtractors empty;

	return empty;
}


const CPackageStaticInfo::AttributeInfos& CPackageStaticInfo::GetAttributeInfos() const
{
	static AttributeInfos empty;

	return empty;
}


const CPackageStaticInfo::SubcomponentInfos& CPackageStaticInfo::GetSubcomponentInfos() const
{
	return m_subcomponentInfos;
}


}//namespace icomp


