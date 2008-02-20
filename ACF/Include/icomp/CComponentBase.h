#ifndef icomp_CComponentBase_included
#define icomp_CComponentBase_included


#include "icomp/IComponent.h"


namespace icomp
{


class CComponentBase: public IComponent
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

private:
	const IComponentContext& m_context;
};


}//namespace icomp


#endif // !icomp_CComponentBase_included


