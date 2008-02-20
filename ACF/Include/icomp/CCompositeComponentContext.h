#ifndef icomp_CCompositeComponentContext_included
#define icomp_CCompositeComponentContext_included


#include "icomp/CComponentContext.h"


namespace icomp
{


class IRegistry;
class IRegistriesManager;


class CCompositeComponentContext: public CComponentContext
{
public:
	typedef CComponentContext BaseClass;

	CCompositeComponentContext(
				const IRegistryElement* elementPtr,
				const IRegistry* registryPtr,
				const CCompositeComponentContext* parentPtr = NULL);

	IRegistriesManager* GetRegistriesManager() const;

	// reimplemented (icomp::IComponentContext)
	virtual IComponent* GetSubComponent(const ::std::string& componentId) const;

private:
	typedef istd::TDelPtr<icomp::IComponent> ComponentPtr;
	typedef ::std::map< ::std::string, ComponentPtr > ComponentMap;
	mutable ComponentMap m_componentMap;
};


}//namespace icomp


#endif // !icomp_CCompositeComponentContext_included


