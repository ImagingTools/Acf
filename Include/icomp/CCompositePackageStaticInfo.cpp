#include "icomp/CCompositePackageStaticInfo.h"


#include "icomp/CComponentAddress.h"


namespace icomp
{


CCompositePackageStaticInfo::CCompositePackageStaticInfo(
			const std::string& packageId,
			const Ids& componentIds,
			const icomp::IComponentEnvironmentManager* managerPtr)

:	m_packageId(packageId),
	m_envManager(*managerPtr)
{
	I_ASSERT(managerPtr != NULL);

	for (		Ids::const_iterator iter = componentIds.begin();
				iter != componentIds.end();
				++iter){
		m_subcomponentInfos.InsertLocal(*iter, ComponentInfo());
	}
}


void CCompositePackageStaticInfo::Reset()
{
	BaseClass::Reset();

	int infosCount = m_subcomponentInfos.GetLocalElementsCount();
	for (int i = 0; i < infosCount; ++i){
		ComponentInfo& info = m_subcomponentInfos.GetLocalValueAt(i);
		info.componentInfoPtr.Reset();
		info.isInitialized = false;
	}
}


//	reimplemented (icomp::IComponentStaticInfo)

const icomp::IComponentStaticInfo::AttributeInfos& CCompositePackageStaticInfo::GetAttributeInfos() const
{
	static CascAttributeInfos empty;

	return empty;
}


icomp::IComponentStaticInfo::Ids CCompositePackageStaticInfo::GetSubcomponentIds() const
{
	Ids retVal;

	m_subcomponentInfos.GetKeys(retVal);

	return retVal;
}


const icomp::IComponentStaticInfo* CCompositePackageStaticInfo::GetSubcomponentInfo(const std::string& subcomponentId) const
{
	ComponentInfo* infoPtr = m_subcomponentInfos.FindLocalElement(subcomponentId);
	if (infoPtr != NULL){
		if (!infoPtr->isInitialized){
			icomp::CComponentAddress address(m_packageId, subcomponentId);

			const icomp::IRegistry* registryPtr = m_envManager.GetRegistry(address);
			if (registryPtr != NULL){
				infoPtr->componentInfoPtr.SetPtr(new icomp::CCompositeComponentStaticInfo(*registryPtr, m_envManager, this));
			}

			infoPtr->isInitialized = true;
		}

		return infoPtr->componentInfoPtr.GetPtr();
	}

	return NULL;

}


}//namespace icomp


