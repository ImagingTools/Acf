#ifndef icomp_CBaseComponentStaticInfo_included
#define icomp_CBaseComponentStaticInfo_included


#include "icomp/IRealComponentStaticInfo.h"
#include "icomp/CComponentStaticInfoBase.h"


namespace icomp
{


/**
	Standard implementation of static info for base component classes.
	The main difference to 'normal' component static info is, that instances of such components cannot be created.
*/
class CBaseComponentStaticInfo:
			public CComponentStaticInfoBase,
			virtual public IRealComponentStaticInfo
{
public:
	typedef CComponentStaticInfoBase BaseClass;

	CBaseComponentStaticInfo(const IRealComponentStaticInfo* baseComponentPtr = NULL);

	/**
		Register interface ID for this static component info.
		This interface ID is used for static check
		if this component can be used to resolve reference dependecy of second one.
	*/
	virtual bool RegisterInterfaceExtractor(const std::string& interfaceName, InterfaceExtractorPtr extractorPtr);
	/**
		Register static attribute info.
		\param	attributeId			ID of attribute.
		\param	attributeInfoPtr	static attribute info object used to describe attribute type and as factory.
									It cannot be NULL.
	*/
	virtual bool RegisterAttributeInfo(const std::string& attributeId, const IAttributeStaticInfo* attributeInfoPtr);

	//	reimplemented (icomp::IRealComponentStaticInfo)
	virtual const InterfaceExtractors& GetInterfaceExtractors() const;

	//	reimplemented (icomp::IComponentStaticInfo)
	virtual const AttributeInfos& GetAttributeInfos() const;
	virtual Ids GetMetaIds(int metaGroupId) const;
	virtual Ids GetSubcomponentIds() const;
	virtual const IComponentStaticInfo* GetSubcomponentInfo(const std::string& subcomponentId) const;

private:
	const IComponentStaticInfo* m_baseComponentPtr;

	CascInterfaceExtractors m_interfaceExtractors;
	CascAttributeInfos m_attributeInfos;
};


}//namespace icomp


#endif // !icomp_CBaseComponentStaticInfo_included


