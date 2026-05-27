// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icomp/CCachedComponentStaticInfo.h>


// ACF includes
#include <icomp/CCachedElementStaticInfo.h>


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


void CCachedComponentStaticInfo::RegisterSubelementInfo(const QByteArray& subelementId, CCachedElementStaticInfo* subelementInfoPtr)
{
	m_subelementInfos[subelementId].reset(subelementInfoPtr);
}


void CCachedComponentStaticInfo::RegisterEmbeddedComponentInfo(const QByteArray& embeddedId, CCachedComponentStaticInfo* componentInfoPtr)
{
	m_embeddedComponentInfos[embeddedId].reset(componentInfoPtr);
}


void CCachedComponentStaticInfo::RegisterAttributeInfo(const QByteArray& attributeId, CCachedAttributeStaticInfo* attributeInfoPtr)
{
	m_attributeInfos[attributeId].reset(attributeInfoPtr);
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
		return foundIter.value().get();
	}

	return NULL;
}


const IComponentStaticInfo* CCachedComponentStaticInfo::GetEmbeddedComponentInfo(const QByteArray& embeddedId) const
{
	EmbeddedComponentInfos::ConstIterator foundIter = m_embeddedComponentInfos.constFind(embeddedId);
	if (foundIter != m_embeddedComponentInfos.constEnd()){
		return foundIter.value().get();
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
		Ids retVal;
		for (		SubelementInfos::const_iterator iter = m_subelementInfos.begin();
					iter != m_subelementInfos.end();
					++iter){
			retVal.insert(iter.key());
		}
		return retVal;
	}
	else if (metaGroupId == MGI_EMBEDDED_COMPONENTS){
		Ids retVal;
		for (		EmbeddedComponentInfos::const_iterator iter = m_embeddedComponentInfos.begin();
					iter != m_embeddedComponentInfos.end();
					++iter){
			retVal.insert(iter.key());
		}
		return retVal;
	}

	return Ids();
}


const IElementStaticInfo* CCachedComponentStaticInfo::GetSubelementInfo(const QByteArray& subcomponentId) const
{
	SubelementInfos::ConstIterator foundIter = m_subelementInfos.constFind(subcomponentId);
	if (foundIter != m_subelementInfos.constEnd()){
		return foundIter.value().get();
	}

	return NULL;
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


