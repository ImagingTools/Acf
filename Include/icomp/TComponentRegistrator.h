#ifndef icomp_TComponentRegistrator_included
#define icomp_TComponentRegistrator_included


#include <string>

#include "icomp/IComponentStaticInfo.h"
#include "icomp/TComponentStaticInfo.h"


namespace icomp
{


/**
	Simple helper class providing registration of template-specified component in package.
*/
template <class Component>
class TComponentRegistrator: public icomp::TComponentStaticInfo<Component>
{
public:
	typedef icomp::TComponentStaticInfo<Component> BaseClass;

	TComponentRegistrator(
				const std::string& componentId,
				icomp::IComponentStaticInfo& staticInfo,
				const istd::CString& description,
				const istd::CString& keywords,
				int category);

	//	reimplemented (icomp::IComponentStaticInfo)
	virtual const istd::CString& GetDescription() const;
	virtual const istd::CString& GetKeywords() const;
	virtual int GetCategory() const;

private:
	std::string m_logicalName;
	istd::CString m_description;
	istd::CString m_keywords;
	int m_category;
};


// public methods

template <class Component>
TComponentRegistrator<Component>::TComponentRegistrator(
			const std::string& componentId,
			icomp::IComponentStaticInfo& staticInfo,
			const istd::CString& description,
			const istd::CString& keywords,
			int category)
:	BaseClass(&Component::InitStaticInfo(NULL)),
	m_description(description),
	m_keywords(keywords),
	m_category(category)
{
	staticInfo.RegisterSubcomponentInfo(componentId, this);
}


//	reimplemented (icomp::IComponentStaticInfo)

template <class Component>
const istd::CString& TComponentRegistrator<Component>::GetDescription() const
{
	return m_description;
}


template <class Component>
const istd::CString& TComponentRegistrator<Component>::GetKeywords() const
{
	return m_keywords;
}


template <class Component>
int TComponentRegistrator<Component>::GetCategory() const
{
	return m_category;
}


}//namespace icomp


#endif // !icomp_TComponentRegistrator_included


