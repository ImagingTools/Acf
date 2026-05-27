// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icomp/CCachedAttributeStaticInfo.h>


namespace icomp
{


// public methods

CCachedAttributeStaticInfo::CCachedAttributeStaticInfo(
			const QByteArray& id,
			const QString& description,
			const QByteArray& typeId,
			int flags)
:	m_id(id),
	m_description(description),
	m_typeId(typeId),
	m_attributeFlags(flags)
{
}


void CCachedAttributeStaticInfo::AddRelatedMetaId(int metaGroupId, const QByteArray& id, int flags)
{
	m_relatedIdsMap[metaGroupId][id] = flags;
}


void CCachedAttributeStaticInfo::SetDefaultValue(iser::IObject* defaultValuePtr)
{
	m_defaultValuePtr.reset(defaultValuePtr);
}


// reimplemented (icomp::IAttributeStaticInfo)

IElementStaticInfo::Ids CCachedAttributeStaticInfo::GetRelatedMetaIds(int metaGroupId, int flags, int flagsMask) const
{
	IElementStaticInfo::Ids retVal;

	RelatedIdsMap::ConstIterator foundGroupIter = m_relatedIdsMap.constFind(metaGroupId);
	if (foundGroupIter != m_relatedIdsMap.constEnd()){
		const IdsToFlagsMap& groupMap = foundGroupIter.value();

		for (		IdsToFlagsMap::const_iterator iter = groupMap.begin();
					iter != groupMap.end();
					++iter){
			int idFlags = iter.value();

			if ((idFlags & flagsMask) == flags){
				retVal.insert(iter.key());
			}
		}
	}

	return retVal;
}


// reimplemented (iattr::IAttributeMetaInfo)

QString CCachedAttributeStaticInfo::GetAttributeDescription() const
{
	return m_description;
}


const iser::IObject* CCachedAttributeStaticInfo::GetAttributeDefaultValue() const
{
	return m_defaultValuePtr.get();
}


QByteArray CCachedAttributeStaticInfo::GetAttributeTypeId() const
{
	return m_typeId;
}


int CCachedAttributeStaticInfo::GetAttributeFlags() const
{
	return m_attributeFlags;
}


} // namespace icomp


