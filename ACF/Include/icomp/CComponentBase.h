#ifndef icomp_CComponentBase_included
#define icomp_CComponentBase_included


#include "icomp/IComponent.h"
#include "icomp/TComponentStaticInfo.h"
#include "icomp/TInterfaceRegistrator.h"


namespace icomp
{


class CComponentBase: virtual public IComponent
{
public:
	/**
		Create component and assign it to specific context.
		\param	contextPtr	pointer to context of this component. It cannot be NULL.
	*/
	CComponentBase(const IComponentContext* contextPtr);

	// reimplemented (icomp::IComponent)
	virtual void* GetInterface(const type_info& interfaceType, const ::std::string& subId = "");
	virtual const IComponentContext& GetComponentContext() const;

	static icomp::IComponentStaticInfo& GetStaticInfo();

private:
	const IComponentContext& m_context;
};


}//namespace icomp


#endif // !icomp_CComponentBase_included


