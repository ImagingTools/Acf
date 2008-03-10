#ifndef icomp_TAttributeStaticInfo_included
#define icomp_TAttributeStaticInfo_included


#include "icomp/IAttributeStaticInfo.h"


namespace icomp
{


template <class Attribute>
class TAttributeStaticInfo: virtual public IAttributeStaticInfo
{
public:
	TAttributeStaticInfo(icomp::IComponentStaticInfo& staticInfo, const ::std::string& id, bool isObligatory);

	// reimplemented (icomp::IAttributeStaticInfo)
	virtual iser::ISerializable* CreateAttribute() const;
	virtual const ::std::string& GetAttributeId() const;
	virtual const type_info& GetAttributeType() const;
	virtual bool IsObligatory() const;

private:
	bool m_isObligatory;
	const ::std::string m_id;
};


// public methods

template <class Attribute>
TAttributeStaticInfo<Attribute>::TAttributeStaticInfo(icomp::IComponentStaticInfo& staticInfo, const ::std::string& id, bool isObligatory)
:	m_isObligatory(isObligatory), m_id(id)
{
	staticInfo.RegisterAttributeInfo(this);
}


// reimplemented (icomp::IAttributeStaticInfo)

template <class Attribute>
iser::ISerializable* TAttributeStaticInfo<Attribute>::CreateAttribute() const
{
	return new Attribute();
}


template <class Attribute>
const ::std::string& TAttributeStaticInfo<Attribute>::GetAttributeId() const
{
	return m_id;
}


template <class Attribute>
const type_info& TAttributeStaticInfo<Attribute>::GetAttributeType() const
{
	return typeid(Attribute);
}


template <class Attribute>
bool TAttributeStaticInfo<Attribute>::IsObligatory() const
{
	return m_isObligatory;
}


}//namespace icomp


#endif // !icomp_TAttributeStaticInfo_included


