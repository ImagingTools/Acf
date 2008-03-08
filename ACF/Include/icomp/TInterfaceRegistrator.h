#ifndef icomp_TInterfaceRegistrator_included
#define icomp_TInterfaceRegistrator_included


#include "icomp/IComponentStaticInfo.h"


namespace icomp
{


/**
	Simple helper class providing registration of template-specified interface.
*/
template <class Interface>
class TInterfaceRegistrator
{
public:
	TInterfaceRegistrator(icomp::IComponentStaticInfo& staticInfo);

protected:
	static void* InterfaceExtractor(IComponent* componentPtr);
};


// protected methods

template <class Interface>
void* TInterfaceRegistrator<Interface>::InterfaceExtractor(IComponent* componentPtr)
{
	return dynamic_cast<Interface>(componentPtr);
}


// public methods

template <class Interface>
TInterfaceRegistrator<Interface>::TInterfaceRegistrator(icomp::IComponentStaticInfo& staticInfo)
{
	staticInfo.RegisterInterfaceExtractor(typeid(Interface).name(), TInterfaceRegistrator<Interface>::InterfaceExtractor);
}


}//namespace icomp


#endif // !icomp_TInterfaceRegistrator_included


