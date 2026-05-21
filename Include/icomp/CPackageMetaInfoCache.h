// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <istd/TDelPtr.h>
#include <icomp/CPackageStaticInfo.h>


class QJsonObject;


namespace icomp
{


class CCachedComponentStaticInfo;
class CCachedAttributeStaticInfo;


/**
	Provides file-based JSON caching of package meta-information.
	This cache allows skipping DLL loading when only component
	meta-information (descriptions, keywords, interfaces, attributes) is needed.

	The cache is invalidated when the DLL file's timestamp or size changes.
*/
class CPackageMetaInfoCache
{
public:
	/**
		Check if a cache file exists and is still valid for the given package file.
		Validation is based on last-modified timestamp and file size.
	*/
	static bool IsCacheValid(const QString& packageFilePath, const QString& cacheFilePath);

	/**
		Load package meta-info from a JSON cache file.
		Returns a newly created CPackageStaticInfo with CCachedComponentStaticInfo instances,
		or NULL if loading fails.
		The caller takes ownership of the returned object.
	*/
	static CPackageStaticInfo* LoadFromCache(const QString& cacheFilePath);

	/**
		Save package meta-info to a JSON cache file.
		Serializes the package and its embedded components to JSON.
		Uses atomic write (temp file + rename) for safety.
	*/
	static bool SaveToCache(const QString& cacheFilePath, const QString& packageFilePath, const CPackageStaticInfo* packageInfo);

	/**
		Compute the cache file path for a given package file.
		If cacheDir is empty, the cache file is placed next to the package file.
	*/
	static QString GetCacheFilePath(const QString& packageFilePath, const QString& cacheDir = QString());

private:
	static QJsonObject SerializeComponent(const IComponentStaticInfo* componentInfo);
	static QJsonObject SerializeAttribute(const IAttributeStaticInfo* attributeInfo);

	static CCachedComponentStaticInfo* DeserializeComponent(const QJsonObject& json);
	static CCachedAttributeStaticInfo* DeserializeAttribute(const QByteArray& attributeId, const QJsonObject& json);
};


} // namespace icomp


