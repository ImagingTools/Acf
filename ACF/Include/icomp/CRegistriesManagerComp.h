#ifndef icomp_CFileRegistriesManagerBase_included
#define icomp_CFileRegistriesManagerBase_included


#include <string>
#include <map>

#include "istd/IPolymorphic.h"
#include "istd/TDelPtr.h"

#include "iser/IArchive.h"
#include "iser/IFileLoader.h"

#include "icomp/IRegistry.h"
#include "icomp/IRegistriesManager.h"
#include "icomp/IComponentStaticInfo.h"
#include "icomp/CComponentBase.h"


namespace icomp
{


class CRegistriesManagerComp: public icomp::CComponentBase, virtual public IRegistriesManager
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CRegistriesManagerComp)
		I_REGISTER_INTERFACE(IRegistriesManager)
		I_ASSIGN(m_componentsFactoryCompPtr, "ComponentsFactory", "Component meta info used to factory registry attributes", true, "ComponentsFactory");
		I_ASSIGN(m_registryLoaderCompPtr, "RegistryLoader", "Loader used to read registry", true, "RegistryLoader")
	I_END_COMPONENT

	const IRegistry* GetRegistryFromFile(const istd::CString& path) const;

	// reimplemented (icomp::IRegistriesManager)
	virtual const IRegistry* GetRegistry(const icomp::CComponentAddress& address, const IRegistry* contextPtr) const;

private:
	typedef std::map<istd::CString, istd::TDelPtr<IRegistry> > RegistriesMap;
	typedef std::map<const IRegistry*, istd::CString> InvRegistriesMap;

	mutable RegistriesMap m_registriesMap;
	mutable InvRegistriesMap m_invRegistriesMap;

	I_REF(IComponentStaticInfo, m_componentsFactoryCompPtr);
	I_REF(iser::IFileLoader, m_registryLoaderCompPtr);
};


}//namespace icomp


#endif // !icomp_CFileRegistriesManagerBase_included


