#ifndef icomp_CComponentStaticInfoBase_included
#define icomp_CComponentStaticInfoBase_included


#include "istd/TCascadedMap.h"

#include "icomp/IComponentStaticInfo.h"


namespace icomp
{


class CComponentStaticInfoBase: virtual public IComponentStaticInfo
{
public:
	typedef istd::TCascadedMap< std::string, const IComponentStaticInfo*> SubcomponentInfos;

	const SubcomponentInfos& GetSubcomponentInfos() const;

	void Reset();

	/**
		Register static subcomponent info.
		\param	subcomponentId		ID of this subcomponent.
		\param	componentInfoPtr	static subcomponent info object used to describe subcomponent type and as factory.
									It cannot be NULL.
	*/
	virtual bool RegisterSubcomponentInfo(const std::string& subcomponentId, const IComponentStaticInfo* componentInfoPtr);

	// reimplemented (icomp::IComponentStaticInfo)
	virtual int GetComponentType() const;
	virtual IComponent* CreateComponent() const;
	virtual Ids GetMetaIds(int metaGroupId) const;
	virtual Ids GetSubcomponentIds() const;
	virtual const IComponentStaticInfo* GetSubcomponentInfo(const std::string& subcomponentId) const;
	virtual const istd::CString& GetDescription() const;
	virtual const istd::CString& GetKeywords() const;

protected:
	typedef istd::TCascadedMap<std::string, InterfaceExtractorPtr> CascInterfaceExtractors;
	typedef istd::TCascadedMap<std::string, const IAttributeStaticInfo*> CascAttributeInfos;

	void ResetSubcomponentList();

private:
	SubcomponentInfos m_subcomponentInfos;
};


// inline methods

inline const CComponentStaticInfoBase::SubcomponentInfos& CComponentStaticInfoBase::GetSubcomponentInfos() const
{
	return m_subcomponentInfos;
}


inline const istd::CString& CComponentStaticInfoBase::GetDescription() const
{
	return istd::CString::GetEmpty();
}


inline const istd::CString& CComponentStaticInfoBase::GetKeywords() const
{
	return istd::CString::GetEmpty();
}


}//namespace icomp


#endif // !icomp_CComponentStaticInfoBase_included


