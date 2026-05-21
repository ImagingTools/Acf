// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icomp/CPackageMetaInfoCache.h>


// Qt includes
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDataStream>
#include <QtCore/QPair>
#include <QtCore/QSaveFile>

// ACF includes
#include <icomp/CCachedComponentStaticInfo.h>
#include <icomp/CCachedAttributeStaticInfo.h>
#include <icomp/IAttributeStaticInfo.h>


namespace icomp
{


// static constants

static const quint32 s_cacheMagic = 0x41434643; // "ACFC"
static const qint32 s_cacheVersion = 2;


// public static methods

CPackageStaticInfo* CPackageMetaInfoCache::LoadFromCacheIfValid(const QString& packageFilePath, const QString& cacheFilePath)
{
	// Strategy 5: Fast filesystem-based pre-check using modification times.
	// This avoids opening the cache file at all when the package is newer.
	QFileInfo cacheFileInfo(cacheFilePath);
	if (!cacheFileInfo.exists()){
		return NULL;
	}

	QFileInfo packageFileInfo(packageFilePath);
	if (!packageFileInfo.exists()){
		return NULL;
	}

	if (packageFileInfo.lastModified() > cacheFileInfo.lastModified()){
		return NULL;
	}

	// Strategy 1 + 3: Single file open with binary format for combined validation + loading.
	QFile cacheFile(cacheFilePath);
	if (!cacheFile.open(QIODevice::ReadOnly)){
		return NULL;
	}

	QDataStream stream(&cacheFile);
	stream.setVersion(QDataStream::Qt_5_0);

	// Read and validate header
	quint32 magic;
	qint32 version;
	stream >> magic >> version;
	if (magic != s_cacheMagic || version != s_cacheVersion){
		return NULL;
	}

	// Read stored validation data
	QString storedLastModified;
	qint64 storedFileSize;
	stream >> storedLastModified >> storedFileSize;

	if (stream.status() != QDataStream::Ok){
		return NULL;
	}

	// Validate against current package file
	QString currentLastModified = packageFileInfo.lastModified().toUTC().toString(Qt::ISODate);
	qint64 currentFileSize = packageFileInfo.size();

	if (storedLastModified != currentLastModified || storedFileSize != currentFileSize){
		return NULL;
	}

	// Read package-level info
	QString packageDescription;
	QString packageKeywords;
	stream >> packageDescription >> packageKeywords;

	if (stream.status() != QDataStream::Ok){
		return NULL;
	}

	CPackageStaticInfo* packageInfo = new CPackageStaticInfo(packageDescription, packageKeywords);

	// Read components
	qint32 componentCount;
	stream >> componentCount;

	for (qint32 i = 0; i < componentCount; ++i){
		QByteArray componentId;
		stream >> componentId;

		CCachedComponentStaticInfo* componentInfo = DeserializeComponent(stream);
		if (componentInfo != NULL && stream.status() == QDataStream::Ok){
			packageInfo->RegisterCachedComponentInfo(componentId, componentInfo);
		}
		else{
			delete componentInfo;
			delete packageInfo;
			return NULL;
		}
	}

	if (stream.status() != QDataStream::Ok){
		delete packageInfo;
		return NULL;
	}

	return packageInfo;
}


bool CPackageMetaInfoCache::SaveToCache(const QString& cacheFilePath, const QString& packageFilePath, const CPackageStaticInfo* packageInfo)
{
	if (packageInfo == NULL){
		return false;
	}

	QFileInfo packageFileInfo(packageFilePath);

	// Write atomically using QSaveFile
	QSaveFile saveFile(cacheFilePath);
	if (!saveFile.open(QIODevice::WriteOnly)){
		return false;
	}

	QDataStream stream(&saveFile);
	stream.setVersion(QDataStream::Qt_5_0);

	// Write header
	stream << s_cacheMagic << s_cacheVersion;

	// Write validation data
	stream << packageFileInfo.lastModified().toUTC().toString(Qt::ISODate);
	stream << packageFileInfo.size();

	// Write package-level info
	stream << packageInfo->GetDescription();
	stream << packageInfo->GetKeywords();

	// Write components - only count non-NULL entries
	IElementStaticInfo::Ids componentIds = packageInfo->GetMetaIds(IComponentStaticInfo::MGI_EMBEDDED_COMPONENTS);

	// Pre-collect valid components to write accurate count
	QList<QPair<QByteArray, const IComponentStaticInfo*>> validComponents;
	for (		IElementStaticInfo::Ids::const_iterator it = componentIds.constBegin();
				it != componentIds.constEnd();
				++it){
		const IComponentStaticInfo* componentInfo = packageInfo->GetEmbeddedComponentInfo(*it);
		if (componentInfo != NULL){
			validComponents.append(qMakePair(*it, componentInfo));
		}
	}

	stream << qint32(validComponents.size());

	for (		QList<QPair<QByteArray, const IComponentStaticInfo*>>::const_iterator it = validComponents.constBegin();
				it != validComponents.constEnd();
				++it){
		stream << it->first;
		SerializeComponent(stream, it->second);
	}

	if (stream.status() != QDataStream::Ok){
		return false;
	}

	return saveFile.commit();
}


QString CPackageMetaInfoCache::GetCacheFilePath(const QString& packageFilePath, const QString& cacheDir)
{
	QFileInfo fileInfo(packageFilePath);
	QString cacheFileName = fileInfo.fileName() + ".cache.bin";

	if (cacheDir.isEmpty()){
		return fileInfo.absolutePath() + QDir::separator() + cacheFileName;
	}
	else{
		return cacheDir + QDir::separator() + cacheFileName;
	}
}


// private static methods

void CPackageMetaInfoCache::SerializeComponent(QDataStream& stream, const IComponentStaticInfo* componentInfo)
{
	stream << componentInfo->GetDescription();
	stream << componentInfo->GetKeywords();
	stream << qint32(componentInfo->GetComponentType());

	// Interfaces
	IElementStaticInfo::Ids interfaceIds = componentInfo->GetMetaIds(IElementStaticInfo::MGI_INTERFACES);
	stream << qint32(interfaceIds.size());
	for (		IElementStaticInfo::Ids::const_iterator it = interfaceIds.constBegin();
				it != interfaceIds.constEnd();
				++it){
		stream << *it;
	}

	// Subelements
	IElementStaticInfo::Ids subelementIds = componentInfo->GetMetaIds(IElementStaticInfo::MGI_SUBELEMENTS);
	stream << qint32(subelementIds.size());
	for (		IElementStaticInfo::Ids::const_iterator it = subelementIds.constBegin();
				it != subelementIds.constEnd();
				++it){
		stream << *it;
	}

	// Attributes - only write non-NULL entries
	iattr::IAttributesProvider::AttributeIds attributeIds = componentInfo->GetAttributeMetaIds();

	QList<QPair<QByteArray, const IAttributeStaticInfo*>> validAttributes;
	for (		iattr::IAttributesProvider::AttributeIds::const_iterator it = attributeIds.constBegin();
				it != attributeIds.constEnd();
				++it){
		const IAttributeStaticInfo* attrInfo = componentInfo->GetAttributeInfo(*it);
		if (attrInfo != NULL){
			validAttributes.append(qMakePair(*it, attrInfo));
		}
	}

	stream << qint32(validAttributes.size());
	for (		QList<QPair<QByteArray, const IAttributeStaticInfo*>>::const_iterator it = validAttributes.constBegin();
				it != validAttributes.constEnd();
				++it){
		stream << it->first;
		SerializeAttribute(stream, it->second);
	}
}


void CPackageMetaInfoCache::SerializeAttribute(QDataStream& stream, const IAttributeStaticInfo* attributeInfo)
{
	stream << attributeInfo->GetAttributeDescription();
	stream << attributeInfo->GetAttributeTypeId();
	stream << qint32(attributeInfo->GetAttributeFlags());

	// Collect related meta groups that have data
	QList<int> activeGroups;
	for (int metaGroupId = IElementStaticInfo::MGI_INTERFACES; metaGroupId <= IComponentStaticInfo::MGI_LAST; ++metaGroupId){
		IElementStaticInfo::Ids relatedIds = attributeInfo->GetRelatedMetaIds(metaGroupId, 0, 0);
		if (!relatedIds.isEmpty()){
			activeGroups.append(metaGroupId);
		}
	}

	stream << qint32(activeGroups.size());

	for (		QList<int>::const_iterator groupIt = activeGroups.constBegin();
				groupIt != activeGroups.constEnd();
				++groupIt){
		int metaGroupId = *groupIt;
		stream << qint32(metaGroupId);

		IElementStaticInfo::Ids relatedIds = attributeInfo->GetRelatedMetaIds(metaGroupId, 0, 0);

		// Pre-fetch flag-specific ID sets for this meta group
		IElementStaticInfo::Ids refIds = attributeInfo->GetRelatedMetaIds(metaGroupId, IAttributeStaticInfo::AF_REFERENCE, IAttributeStaticInfo::AF_REFERENCE);
		IElementStaticInfo::Ids factIds = attributeInfo->GetRelatedMetaIds(metaGroupId, IAttributeStaticInfo::AF_FACTORY, IAttributeStaticInfo::AF_FACTORY);
		IElementStaticInfo::Ids oblIds = attributeInfo->GetRelatedMetaIds(metaGroupId, iattr::IAttributeMetaInfo::AF_OBLIGATORY, iattr::IAttributeMetaInfo::AF_OBLIGATORY);
		IElementStaticInfo::Ids nullIds = attributeInfo->GetRelatedMetaIds(metaGroupId, iattr::IAttributeMetaInfo::AF_NULLABLE, iattr::IAttributeMetaInfo::AF_NULLABLE);

		stream << qint32(relatedIds.size());

		for (		IElementStaticInfo::Ids::const_iterator it = relatedIds.constBegin();
					it != relatedIds.constEnd();
					++it){
			int idFlags = 0;

			if (refIds.contains(*it)){
				idFlags |= IAttributeStaticInfo::AF_REFERENCE;
			}
			if (factIds.contains(*it)){
				idFlags |= IAttributeStaticInfo::AF_FACTORY;
			}
			if (oblIds.contains(*it)){
				idFlags |= iattr::IAttributeMetaInfo::AF_OBLIGATORY;
			}
			if (nullIds.contains(*it)){
				idFlags |= iattr::IAttributeMetaInfo::AF_NULLABLE;
			}

			stream << *it << qint32(idFlags);
		}
	}
}


CCachedComponentStaticInfo* CPackageMetaInfoCache::DeserializeComponent(QDataStream& stream)
{
	QString description;
	QString keywords;
	qint32 componentType;
	stream >> description >> keywords >> componentType;

	if (stream.status() != QDataStream::Ok){
		return NULL;
	}

	CCachedComponentStaticInfo* componentInfo = new CCachedComponentStaticInfo(description, keywords, componentType);

	// Interfaces
	qint32 interfaceCount;
	stream >> interfaceCount;
	for (qint32 i = 0; i < interfaceCount; ++i){
		QByteArray interfaceId;
		stream >> interfaceId;
		componentInfo->AddInterfaceId(interfaceId);
	}

	// Subelements
	qint32 subelementCount;
	stream >> subelementCount;
	for (qint32 i = 0; i < subelementCount; ++i){
		QByteArray subelementId;
		stream >> subelementId;
		componentInfo->AddSubelementId(subelementId);
	}

	// Attributes
	qint32 attributeCount;
	stream >> attributeCount;
	for (qint32 i = 0; i < attributeCount; ++i){
		QByteArray attributeId;
		stream >> attributeId;

		CCachedAttributeStaticInfo* attrInfo = DeserializeAttribute(stream, attributeId);
		if (attrInfo != NULL && stream.status() == QDataStream::Ok){
			componentInfo->RegisterAttributeInfo(attributeId, attrInfo);
		}
		else{
			delete attrInfo;
			delete componentInfo;
			return NULL;
		}
	}

	return componentInfo;
}


CCachedAttributeStaticInfo* CPackageMetaInfoCache::DeserializeAttribute(QDataStream& stream, const QByteArray& attributeId)
{
	QString description;
	QByteArray typeId;
	qint32 flags;
	stream >> description >> typeId >> flags;

	if (stream.status() != QDataStream::Ok){
		return NULL;
	}

	CCachedAttributeStaticInfo* attrInfo = new CCachedAttributeStaticInfo(attributeId, description, typeId, flags);

	// Related meta groups
	qint32 groupCount;
	stream >> groupCount;

	for (qint32 g = 0; g < groupCount; ++g){
		qint32 metaGroupId;
		qint32 idCount;
		stream >> metaGroupId >> idCount;

		for (qint32 j = 0; j < idCount; ++j){
			QByteArray id;
			qint32 idFlags;
			stream >> id >> idFlags;

			attrInfo->AddRelatedMetaId(metaGroupId, id, idFlags);
		}
	}

	return attrInfo;
}


} // namespace icomp


