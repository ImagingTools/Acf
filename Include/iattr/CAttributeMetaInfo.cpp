#include "iattr/CAttributeMetaInfo.h"


namespace iattr
{


void CAttributeMetaInfo::SetAttributeDescription(const QString& description)
{
	m_description = description;
}


void CAttributeMetaInfo::SetAttributeTypeId(const QByteArray& typeId)
{
	m_attributeTypeId = typeId;
}


void CAttributeMetaInfo::SetAttributeFlags(int attributeFlags)
{
	m_attributeFlags = attributeFlags;
}


void CAttributeMetaInfo::SetAttributeDefaultValue(const iser::IObject* defaultValuePtr, bool releaseFlag)
{
	m_defaultValuePtr.SetPtr(defaultValuePtr, releaseFlag);
}


// reimplemented (iattr::IAttributeMetaInfo)

QString CAttributeMetaInfo::GetAttributeDescription() const
{
	return m_description;
}


const iser::IObject* CAttributeMetaInfo::GetAttributeDefaultValue() const
{
	return m_defaultValuePtr.GetPtr();
}


QByteArray CAttributeMetaInfo::GetAttributeTypeId() const
{
	return m_attributeTypeId;
}


int CAttributeMetaInfo::GetAttributeFlags() const
{
	return m_attributeFlags;
}


} // namespace iattr


