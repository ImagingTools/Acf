// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iattr/CAttributeMetaInfo.h>


namespace iattr
{


CAttributeMetaInfo::CAttributeMetaInfo(const QByteArray& typeId)
:	m_attributeTypeId(typeId),
	m_attributeFlags(0)
{
}


void CAttributeMetaInfo::SetAttributeTypeId(const QByteArray& typeId)
{
	m_attributeTypeId = typeId;
}


void CAttributeMetaInfo::SetAttributeDescription(const QString& description)
{
	m_description = description;
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


