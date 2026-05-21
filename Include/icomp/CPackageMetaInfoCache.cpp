// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icomp/CPackageMetaInfoCache.h>


// Qt includes
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QSaveFile>

// ACF includes
#include <icomp/CCachedComponentStaticInfo.h>
#include <icomp/CCachedAttributeStaticInfo.h>
#include <icomp/IAttributeStaticInfo.h>


namespace icomp
{


// static constants

static const char s_packageFileKey[] = "packageFile";
static const char s_lastModifiedKey[] = "lastModified";
static const char s_fileSizeKey[] = "fileSize";
static const char s_packageKey[] = "package";
static const char s_componentsKey[] = "components";
static const char s_descriptionKey[] = "description";
static const char s_keywordsKey[] = "keywords";
static const char s_componentTypeKey[] = "componentType";
static const char s_interfacesKey[] = "interfaces";
static const char s_attributesKey[] = "attributes";
static const char s_subelementsKey[] = "subelements";
static const char s_typeIdKey[] = "typeId";
static const char s_flagsKey[] = "flags";
static const char s_relatedInterfacesKey[] = "relatedInterfaces";
static const int s_cacheVersion = 1;
static const char s_cacheVersionKey[] = "cacheVersion";


// public static methods

bool CPackageMetaInfoCache::IsCacheValid(const QString& packageFilePath, const QString& cacheFilePath)
{
	QFileInfo cacheFileInfo(cacheFilePath);
	if (!cacheFileInfo.exists()){
		return false;
	}

	QFileInfo packageFileInfo(packageFilePath);
	if (!packageFileInfo.exists()){
		return false;
	}

	QFile cacheFile(cacheFilePath);
	if (!cacheFile.open(QIODevice::ReadOnly)){
		return false;
	}

	QJsonDocument doc = QJsonDocument::fromJson(cacheFile.readAll());
	if (doc.isNull() || !doc.isObject()){
		return false;
	}

	QJsonObject root = doc.object();

	if (root[s_cacheVersionKey].toInt() != s_cacheVersion){
		return false;
	}

	QString storedLastModified = root[s_lastModifiedKey].toString();
	qint64 storedFileSize = qint64(root[s_fileSizeKey].toDouble());

	QString currentLastModified = packageFileInfo.lastModified().toUTC().toString(Qt::ISODate);
	qint64 currentFileSize = packageFileInfo.size();

	return (storedLastModified == currentLastModified) && (storedFileSize == currentFileSize);
}


CPackageStaticInfo* CPackageMetaInfoCache::LoadFromCache(const QString& cacheFilePath)
{
	QFile cacheFile(cacheFilePath);
	if (!cacheFile.open(QIODevice::ReadOnly)){
		return NULL;
	}

	QJsonDocument doc = QJsonDocument::fromJson(cacheFile.readAll());
	if (doc.isNull() || !doc.isObject()){
		return NULL;
	}

	QJsonObject root = doc.object();

	if (root[s_cacheVersionKey].toInt() != s_cacheVersion){
		return NULL;
	}

	// Read package-level info
	QJsonObject packageObj = root[s_packageKey].toObject();
	QString packageDescription = packageObj[s_descriptionKey].toString();
	QString packageKeywords = packageObj[s_keywordsKey].toString();

	CPackageStaticInfo* packageInfo = new CPackageStaticInfo(packageDescription, packageKeywords);

	// Read components
	QJsonObject componentsObj = root[s_componentsKey].toObject();
	for (QJsonObject::const_iterator it = componentsObj.constBegin(); it != componentsObj.constEnd(); ++it){
		QByteArray componentId = it.key().toUtf8();
		QJsonObject componentObj = it.value().toObject();

		CCachedComponentStaticInfo* componentInfo = DeserializeComponent(componentObj);
		if (componentInfo != NULL){
			packageInfo->RegisterCachedComponentInfo(componentId, componentInfo);
		}
	}

	return packageInfo;
}


bool CPackageMetaInfoCache::SaveToCache(const QString& cacheFilePath, const QString& packageFilePath, const CPackageStaticInfo* packageInfo)
{
	if (packageInfo == NULL){
		return false;
	}

	QFileInfo packageFileInfo(packageFilePath);

	QJsonObject root;
	root[s_cacheVersionKey] = s_cacheVersion;
	root[s_packageFileKey] = packageFilePath;
	root[s_lastModifiedKey] = packageFileInfo.lastModified().toUTC().toString(Qt::ISODate);
	root[s_fileSizeKey] = double(packageFileInfo.size());

	// Package-level info
	QJsonObject packageObj;
	packageObj[s_descriptionKey] = packageInfo->GetDescription();
	packageObj[s_keywordsKey] = packageInfo->GetKeywords();
	root[s_packageKey] = packageObj;

	// Components
	QJsonObject componentsObj;
	IElementStaticInfo::Ids componentIds = packageInfo->GetMetaIds(IComponentStaticInfo::MGI_EMBEDDED_COMPONENTS);
	for (		IElementStaticInfo::Ids::const_iterator it = componentIds.constBegin();
				it != componentIds.constEnd();
				++it){
		const IComponentStaticInfo* componentInfo = packageInfo->GetEmbeddedComponentInfo(*it);
		if (componentInfo != NULL){
			componentsObj[QString::fromUtf8(*it)] = SerializeComponent(componentInfo);
		}
	}
	root[s_componentsKey] = componentsObj;

	// Write atomically using QSaveFile
	QSaveFile saveFile(cacheFilePath);
	if (!saveFile.open(QIODevice::WriteOnly)){
		return false;
	}

	QJsonDocument doc(root);
	saveFile.write(doc.toJson(QJsonDocument::Indented));

	return saveFile.commit();
}


QString CPackageMetaInfoCache::GetCacheFilePath(const QString& packageFilePath, const QString& cacheDir)
{
	QFileInfo fileInfo(packageFilePath);
	QString cacheFileName = fileInfo.fileName() + ".cache.json";

	if (cacheDir.isEmpty()){
		return fileInfo.absolutePath() + QDir::separator() + cacheFileName;
	}
	else{
		return cacheDir + QDir::separator() + cacheFileName;
	}
}


// private static methods

QJsonObject CPackageMetaInfoCache::SerializeComponent(const IComponentStaticInfo* componentInfo)
{
	QJsonObject obj;

	obj[s_descriptionKey] = componentInfo->GetDescription();
	obj[s_keywordsKey] = componentInfo->GetKeywords();
	obj[s_componentTypeKey] = componentInfo->GetComponentType();

	// Interfaces
	QJsonArray interfacesArray;
	IElementStaticInfo::Ids interfaceIds = componentInfo->GetMetaIds(IElementStaticInfo::MGI_INTERFACES);
	for (		IElementStaticInfo::Ids::const_iterator it = interfaceIds.constBegin();
				it != interfaceIds.constEnd();
				++it){
		interfacesArray.append(QString::fromUtf8(*it));
	}
	obj[s_interfacesKey] = interfacesArray;

	// Subelements
	QJsonArray subelementsArray;
	IElementStaticInfo::Ids subelementIds = componentInfo->GetMetaIds(IElementStaticInfo::MGI_SUBELEMENTS);
	for (		IElementStaticInfo::Ids::const_iterator it = subelementIds.constBegin();
				it != subelementIds.constEnd();
				++it){
		subelementsArray.append(QString::fromUtf8(*it));
	}
	obj[s_subelementsKey] = subelementsArray;

	// Attributes
	QJsonObject attributesObj;
	iattr::IAttributesProvider::AttributeIds attributeIds = componentInfo->GetAttributeMetaIds();
	for (		iattr::IAttributesProvider::AttributeIds::const_iterator it = attributeIds.constBegin();
				it != attributeIds.constEnd();
				++it){
		const IAttributeStaticInfo* attrInfo = componentInfo->GetAttributeInfo(*it);
		if (attrInfo != NULL){
			attributesObj[QString::fromUtf8(*it)] = SerializeAttribute(attrInfo);
		}
	}
	obj[s_attributesKey] = attributesObj;

	return obj;
}


QJsonObject CPackageMetaInfoCache::SerializeAttribute(const IAttributeStaticInfo* attributeInfo)
{
	QJsonObject obj;

	obj[s_descriptionKey] = attributeInfo->GetAttributeDescription();
	obj[s_typeIdKey] = QString::fromUtf8(attributeInfo->GetAttributeTypeId());
	obj[s_flagsKey] = attributeInfo->GetAttributeFlags();

	// Related interfaces - serialize for all meta groups that may have data
	QJsonObject relatedObj;
	for (int metaGroupId = IElementStaticInfo::MGI_INTERFACES; metaGroupId <= IComponentStaticInfo::MGI_LAST; ++metaGroupId){
		// Get all related IDs regardless of flags (use 0, 0 to match everything)
		IElementStaticInfo::Ids relatedIds = attributeInfo->GetRelatedMetaIds(metaGroupId, 0, 0);
		if (!relatedIds.isEmpty()){
			QJsonObject groupObj;
			for (		IElementStaticInfo::Ids::const_iterator it = relatedIds.constBegin();
						it != relatedIds.constEnd();
						++it){
				// Try to determine flags for this specific ID
				int idFlags = 0;

				// Test common flag combinations to reconstruct the flags
				if (!attributeInfo->GetRelatedMetaIds(metaGroupId, IAttributeStaticInfo::AF_REFERENCE, IAttributeStaticInfo::AF_REFERENCE).isEmpty()){
					IElementStaticInfo::Ids refIds = attributeInfo->GetRelatedMetaIds(metaGroupId, IAttributeStaticInfo::AF_REFERENCE, IAttributeStaticInfo::AF_REFERENCE);
					if (refIds.contains(*it)){
						idFlags |= IAttributeStaticInfo::AF_REFERENCE;
					}
				}
				if (!attributeInfo->GetRelatedMetaIds(metaGroupId, IAttributeStaticInfo::AF_FACTORY, IAttributeStaticInfo::AF_FACTORY).isEmpty()){
					IElementStaticInfo::Ids factIds = attributeInfo->GetRelatedMetaIds(metaGroupId, IAttributeStaticInfo::AF_FACTORY, IAttributeStaticInfo::AF_FACTORY);
					if (factIds.contains(*it)){
						idFlags |= IAttributeStaticInfo::AF_FACTORY;
					}
				}
				if (!attributeInfo->GetRelatedMetaIds(metaGroupId, iattr::IAttributeMetaInfo::AF_OBLIGATORY, iattr::IAttributeMetaInfo::AF_OBLIGATORY).isEmpty()){
					IElementStaticInfo::Ids oblIds = attributeInfo->GetRelatedMetaIds(metaGroupId, iattr::IAttributeMetaInfo::AF_OBLIGATORY, iattr::IAttributeMetaInfo::AF_OBLIGATORY);
					if (oblIds.contains(*it)){
						idFlags |= iattr::IAttributeMetaInfo::AF_OBLIGATORY;
					}
				}
				if (!attributeInfo->GetRelatedMetaIds(metaGroupId, iattr::IAttributeMetaInfo::AF_NULLABLE, iattr::IAttributeMetaInfo::AF_NULLABLE).isEmpty()){
					IElementStaticInfo::Ids nullIds = attributeInfo->GetRelatedMetaIds(metaGroupId, iattr::IAttributeMetaInfo::AF_NULLABLE, iattr::IAttributeMetaInfo::AF_NULLABLE);
					if (nullIds.contains(*it)){
						idFlags |= iattr::IAttributeMetaInfo::AF_NULLABLE;
					}
				}

				groupObj[QString::fromUtf8(*it)] = idFlags;
			}
			relatedObj[QString::number(metaGroupId)] = groupObj;
		}
	}
	if (!relatedObj.isEmpty()){
		obj[s_relatedInterfacesKey] = relatedObj;
	}

	return obj;
}


CCachedComponentStaticInfo* CPackageMetaInfoCache::DeserializeComponent(const QJsonObject& json)
{
	QString description = json[s_descriptionKey].toString();
	QString keywords = json[s_keywordsKey].toString();
	int componentType = json[s_componentTypeKey].toInt(IComponentStaticInfo::CT_REAL);

	CCachedComponentStaticInfo* componentInfo = new CCachedComponentStaticInfo(description, keywords, componentType);

	// Interfaces
	QJsonArray interfacesArray = json[s_interfacesKey].toArray();
	for (int i = 0; i < interfacesArray.size(); ++i){
		componentInfo->AddInterfaceId(interfacesArray[i].toString().toUtf8());
	}

	// Subelements
	QJsonArray subelementsArray = json[s_subelementsKey].toArray();
	for (int i = 0; i < subelementsArray.size(); ++i){
		componentInfo->AddSubelementId(subelementsArray[i].toString().toUtf8());
	}

	// Attributes
	QJsonObject attributesObj = json[s_attributesKey].toObject();
	for (QJsonObject::const_iterator it = attributesObj.constBegin(); it != attributesObj.constEnd(); ++it){
		QByteArray attributeId = it.key().toUtf8();
		QJsonObject attrObj = it.value().toObject();

		CCachedAttributeStaticInfo* attrInfo = DeserializeAttribute(attributeId, attrObj);
		if (attrInfo != NULL){
			componentInfo->RegisterAttributeInfo(attributeId, attrInfo);
		}
	}

	return componentInfo;
}


CCachedAttributeStaticInfo* CPackageMetaInfoCache::DeserializeAttribute(const QByteArray& attributeId, const QJsonObject& json)
{
	Q_UNUSED(attributeId);

	QString description = json[s_descriptionKey].toString();
	QByteArray typeId = json[s_typeIdKey].toString().toUtf8();
	int flags = json[s_flagsKey].toInt();

	CCachedAttributeStaticInfo* attrInfo = new CCachedAttributeStaticInfo(attributeId, description, typeId, flags);

	// Related interfaces
	QJsonObject relatedObj = json[s_relatedInterfacesKey].toObject();
	for (QJsonObject::const_iterator groupIt = relatedObj.constBegin(); groupIt != relatedObj.constEnd(); ++groupIt){
		int metaGroupId = groupIt.key().toInt();
		QJsonObject groupObj = groupIt.value().toObject();

		for (QJsonObject::const_iterator idIt = groupObj.constBegin(); idIt != groupObj.constEnd(); ++idIt){
			QByteArray id = idIt.key().toUtf8();
			int idFlags = idIt.value().toInt();

			attrInfo->AddRelatedMetaId(metaGroupId, id, idFlags);
		}
	}

	return attrInfo;
}


} // namespace icomp


