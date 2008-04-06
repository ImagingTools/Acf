#ifndef icomp_TComponentStaticInfo_included
#define icomp_TComponentStaticInfo_included


#include "icomp/TComponentWrap.h"
#include "icomp/TBaseComponentStaticInfo.h"


namespace icomp
{


template <class Component>
class TComponentStaticInfo: public TBaseComponentStaticInfo<Component>
{
public:
	typedef TBaseComponentStaticInfo<Component> BaseClass;

	TComponentStaticInfo(const IComponentStaticInfo* baseComponentPtr = NULL);

	//	reimplemented (icomp::IComponentStaticInfo)
	virtual IComponent* CreateComponent(const IComponentContext* contextPtr) const;
};


// public methods

template <class Component>
TComponentStaticInfo<Component>::TComponentStaticInfo(const IComponentStaticInfo* baseComponentPtr)
:	BaseClass(baseComponentPtr)
{
}


//	reimplemented (icomp::IComponentStaticInfo)

template <class Component>
IComponent* TComponentStaticInfo<Component>::CreateComponent(const IComponentContext* contextPtr) const
{
	TComponentWrap<Component>* componentPtr = new TComponentWrap<Component>(contextPtr);

	return componentPtr;
}


}//namespace icomp


#endif // !icomp_TComponentStaticInfo_included


