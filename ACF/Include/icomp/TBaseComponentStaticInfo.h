#ifndef icomp_TBaseComponentStaticInfo_included
#define icomp_TBaseComponentStaticInfo_included


#include "icomp/IComponentStaticInfo.h"
#include "icomp/TComponentWrap.h"


namespace icomp
{


/**
	Standard implementation of static info for base component classes.
	The main difference to 'normal' component static info is, that instances of such components cannot be created.
*/
template <class Component>
class TBaseComponentStaticInfo: virtual public IComponentStaticInfo
{
public:
	TBaseComponentStaticInfo(const IComponentStaticInfo* baseComponentPtr = NULL);

	//	reimplemented (icomp::IComponentStaticInfo)
	virtual IComponent* CreateComponent(const IComponentContext* contextPtr) const;
	virtual const InterfaceExtractors& GetInterfaceExtractors() const;
	virtual const AttributeInfos& GetAttributeInfos() const;
	virtual const SubcomponentInfos& GetSubcomponentInfos() const;
	virtual bool RegisterInterfaceExtractor(const std::string& interfaceId, InterfaceExtractorPtr extractorPtr);
	virtual bool RegisterAttributeInfo(const std::string& attributeId, const IAttributeStaticInfo* attributeInfoPtr);
	virtual bool RegisterSubcomponentInfo(const std::string& subcomponentId, const IComponentStaticInfo* componentInfoPtr);

private:
	const IComponentStaticInfo* m_baseComponentPtr;

	InterfaceExtractors m_interfaceExtractors;
	AttributeInfos m_attributeInfos;
	SubcomponentInfos m_subcomponentInfos;
};


// public methods

template <class Component>
TBaseComponentStaticInfo<Component>::TBaseComponentStaticInfo(const IComponentStaticInfo* baseComponentPtr)
:	m_baseComponentPtr(baseComponentPtr)
{
	if (baseComponentPtr != NULL){
		m_interfaceExtractors.SetParent(&baseComponentPtr->GetInterfaceExtractors());
		m_attributeInfos.SetParent(&baseComponentPtr->GetAttributeInfos());
		m_subcomponentInfos.SetParent(&baseComponentPtr->GetSubcomponentInfos());
	}
}


//	reimplemented (icomp::IComponentStaticInfo)

template <class Component>
IComponent* TBaseComponentStaticInfo<Component>::CreateComponent(const IComponentContext* /*contextPtr*/) const
{
	I_CRITICAL();	// trying to create abstract base component.

	return NULL;
}


template <class Component>
const IComponentStaticInfo::InterfaceExtractors& TBaseComponentStaticInfo<Component>::GetInterfaceExtractors() const
{
	return m_interfaceExtractors;
}


template <class Component>
const IComponentStaticInfo::AttributeInfos& TBaseComponentStaticInfo<Component>::GetAttributeInfos() const
{
	return m_attributeInfos;
}


template <class Component>
const IComponentStaticInfo::SubcomponentInfos& TBaseComponentStaticInfo<Component>::GetSubcomponentInfos() const
{
	return m_subcomponentInfos;
}


template <class Component>
bool TBaseComponentStaticInfo<Component>::RegisterInterfaceExtractor(const std::string& interfaceId, InterfaceExtractorPtr extractorPtr)
{
	return m_interfaceExtractors.InsertLocal(interfaceId, extractorPtr);
}


template <class Component>
bool TBaseComponentStaticInfo<Component>::RegisterAttributeInfo(const std::string& attributeId, const IAttributeStaticInfo* attributeInfoPtr)
{
	return m_attributeInfos.InsertLocal(attributeId, attributeInfoPtr);
}


template <class Component>
bool TBaseComponentStaticInfo<Component>::RegisterSubcomponentInfo(const std::string& subcomponentId, const IComponentStaticInfo* componentInfoPtr)
{
	return m_subcomponentInfos.InsertLocal(subcomponentId, componentInfoPtr);
}


// public methods of embedded class InterfaceRegistrator

template <class Component>
TBaseComponentStaticInfo<Component>::InterfaceRegistrator::InterfaceRegistrator(icomp::IComponentStaticInfo* staticInfoPtr)
{
	if (staticInfoPtr != NULL){
		staticInfoPtr->RegisterInterfaceExtractor(typeid(Component).name());
	}
}


}//namespace icomp


#endif // !icomp_TBaseComponentStaticInfo_included


