#ifndef icomp_CCompositeComponentStaticInfo_included
#define icomp_CCompositeComponentStaticInfo_included


#include "icomp/CBaseComponentStaticInfo.h"


namespace icomp
{


class IRegistry;


class CCompositeComponentStaticInfo: public CBaseComponentStaticInfo
{
public:
	CCompositeComponentStaticInfo(const IRegistry& registry);

	//	reimplemented (icomp::IComponentStaticInfo)
	virtual int GetComponentType() const;
	virtual IComponent* CreateComponent() const;
	virtual const istd::CString& GetDescription() const;
	virtual const istd::CString& GetKeywords() const;

private:
	istd::CString m_description;
	istd::CString m_keywords;
};


}//namespace icomp


#endif // !icomp_CCompositeComponentStaticInfo_included


