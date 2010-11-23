#ifndef icomp_TComponentStaticInfo_included
#define icomp_TComponentStaticInfo_included


#include "icomp/TComponentWrap.h"
#include "icomp/CBaseComponentStaticInfo.h"


namespace icomp
{


template <class Component>
class TComponentStaticInfo: public CBaseComponentStaticInfo
{
public:
	typedef CBaseComponentStaticInfo BaseClass;

	TComponentStaticInfo(const IRealComponentStaticInfo* baseComponentPtr = NULL);

	//	reimplemented (icomp::IComponentStaticInfo)
	virtual int GetComponentType() const;
	virtual IComponent* CreateComponent() const;
};


// public methods

template <class Component>
TComponentStaticInfo<Component>::TComponentStaticInfo(const IRealComponentStaticInfo* baseComponentPtr)
:	BaseClass(baseComponentPtr)
{
}


//	reimplemented (icomp::IComponentStaticInfo)

template <class Component>
int TComponentStaticInfo<Component>::GetComponentType() const
{
	return CT_REAL;
}


template <class Component>
IComponent* TComponentStaticInfo<Component>::CreateComponent() const
{
	TComponentWrap<Component>* componentPtr = new TComponentWrap<Component>();

	return componentPtr;
}


}//namespace icomp


#endif // !icomp_TComponentStaticInfo_included


