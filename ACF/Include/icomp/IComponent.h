#ifndef icomp_IComponent_included
#define icomp_IComponent_included


#include <string>

#include "istd/IPolymorphic.h"

#include "icomp/icomp.h"


namespace icomp
{


class IComponentContext;


/**
	Main component interface.
*/
class IComponent: virtual public istd::IPolymorphic
{
public:
	/**
		Get access to specified component interface.
	*/
	virtual void* GetInterface(const type_info& interfaceType, const ::std::string& subId = "") = 0;

	/**
		Get access to component context describing all application-specified
		component information loaded from components registry.
	*/
	virtual const IComponentContext& GetComponentContext() const = 0;
};


}//namespace icomp


#endif // !icomp_IComponent_included


