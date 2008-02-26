#ifndef icomp_TInterfaceRegistrator_included
#define icomp_TInterfaceRegistrator_included


#include "icomp/icomp.h"


namespace icomp
{


/**
	Simple helper class providing registration of template-specified interface.
*/
template <class Interface>
class TInterfaceRegistrator
{
public:
	TInterfaceRegistrator(icomp::IComponentStaticInfo* staticInfoPtr);

protected:
	static void* InterfaceExtractor(IComponent* componentPtr);
};


// public methods

template <class Interface>
TInterfaceRegistrator<Interface>::TInterfaceRegistrator(icomp::IComponentStaticInfo& staticInfo)
{
	staticInfo.RegisterInterfaceExtractor(typeid(Interface).name(), InterfaceExtractor);
}


template <class Interface>
static void* TInterfaceRegistrator<Interface>::InterfaceExtractor(IComponent* componentPtr)
{
	return dynamic_cast<Interface>(componentPtr);
}


}//namespace icomp


#endif // !icomp_TInterfaceRegistrator_included


