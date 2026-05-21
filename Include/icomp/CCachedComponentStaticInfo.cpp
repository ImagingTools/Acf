// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icomp/CCachedComponentStaticInfo.h>


namespace icomp
{


// public methods

CCachedComponentStaticInfo::CCachedComponentStaticInfo(
			const QString& description,
			const QString& keywords,
			int componentType)
:	m_description(description),
	m_keywords(keywords),
	m_componentType(componentType)
{
}


void CCachedComponentStaticInfo::AddInterfaceId(const QByteArray& interfaceId)
{
	m_interfaceIds.insert(interfaceId);
}


void CCachedComponentStaticInfo::AddSubelementId(const QByteArray& subelementId)
{
	m_subelementIds.insert(subelementId);
}


void CCachedComponentStaticInfo::RegisterAttributeInfo(const QByteArray& attributeId, CCachedAttributeStaticInfo* attributeInfoPtr)
{
	m_attributeInfos[attributeId].SetPtr(attributeInfoPtr);
}


// reimplemented (icomp::IComponentStaticInfo)

int CCachedComponentStaticInfo::GetComponentType() const
{
	return m_componentType;
}


const IAttributeStaticInfo* CCachedComponentStaticInfo::GetAttributeInfo(const QByteArray& attributeId) const
{
	AttributeInfos::ConstIterator foundIter = m_attributeInfos.constFind(attributeId);
	if (foundIter != m_attributeInfos.constEnd()){
		return foundIter.value().GetPtr();
	}

	return NULL;
}


const QString& CCachedComponentStaticInfo::GetDescription() const
{
	return m_description;
}


const QString& CCachedComponentStaticInfo::GetKeywords() const
{
	return m_keywords;
}


// reimplemented (icomp::IElementStaticInfo)

IElementStaticInfo::Ids CCachedComponentStaticInfo::GetMetaIds(int metaGroupId) const
{
	if (metaGroupId == MGI_INTERFACES){
		return m_interfaceIds;
	}
	else if (metaGroupId == MGI_SUBELEMENTS){
		return m_subelementIds;
	}

	return Ids();
}


// reimplemented (iattr::IAttributesMetaInfoProvider)

iattr::IAttributesProvider::AttributeIds CCachedComponentStaticInfo::GetAttributeMetaIds() const
{
	iattr::IAttributesProvider::AttributeIds retVal;

	for (		AttributeInfos::const_iterator iter = m_attributeInfos.begin();
				iter != m_attributeInfos.end();
				++iter){
		retVal.insert(iter.key());
	}

	return retVal;
}


const iattr::IAttributeMetaInfo* CCachedComponentStaticInfo::GetAttributeMetaInfo(const QByteArray& attributeId) const
{
	return CCachedComponentStaticInfo::GetAttributeInfo(attributeId);
}


} // namespace icomp


