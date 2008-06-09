#include "icomp/CRegistriesManagerComp.h"


#include <string>

#include "istd/IPolymorphic.h"
#include "istd/CStaticServicesProvider.h"

#include "isys/IFileSystem.h"

#include "icomp/CRegistry.h"
#include "icomp/CComponentAddress.h"


namespace icomp
{


const IRegistry* CRegistriesManagerComp::GetRegistryFromFile(const istd::CString& path) const
{
	istd::CString correctedPath = path;

	isys::IFileSystem* fileSystemPtr = istd::GetService<isys::IFileSystem>();
	if (fileSystemPtr != NULL){
		correctedPath = fileSystemPtr->GetNormalizedPath(correctedPath);
	}

	RegistriesMap::const_iterator iter = m_registriesMap.find(correctedPath);

	if (iter != m_registriesMap.end()){
		return iter->second.GetPtr();
	}

	istd::TDelPtr<IRegistry>& mapValue = m_registriesMap[correctedPath];
	if (m_registryLoaderCompPtr.IsValid() && m_componentsFactoryCompPtr.IsValid()){
		istd::TDelPtr<IRegistry> newRegistryPtr(new CRegistry(m_componentsFactoryCompPtr.GetPtr()));
		if (m_registryLoaderCompPtr->LoadFromFile(*newRegistryPtr, correctedPath) == iser::IFileLoader::StateOk){
			mapValue.TakeOver(newRegistryPtr);
			m_invRegistriesMap[mapValue.GetPtr()] = correctedPath;

			return mapValue.GetPtr();
		}
	}

	return NULL;
}


// reimplemented (icomp::IRegistriesManager)

const IRegistry* CRegistriesManagerComp::GetRegistry(
			const icomp::CComponentAddress& address,
			const IRegistry* contextPtr) const
{
	istd::CString path;

	if (contextPtr != NULL){
		InvRegistriesMap::const_iterator contextIter = m_invRegistriesMap.find(contextPtr);

		I_ASSERT(contextIter != m_invRegistriesMap.end());

		if (contextIter != m_invRegistriesMap.end()){
			path = contextIter->second + istd::CString("/");
		}
	}

	const std::string& packageId = address.GetPackageId();
	if (!packageId.empty()){
		path += istd::CString(packageId) + istd::CString("/");
	}

	path += istd::CString(address.GetComponentId());

	return GetRegistryFromFile(path);
}


}//namespace icomp


