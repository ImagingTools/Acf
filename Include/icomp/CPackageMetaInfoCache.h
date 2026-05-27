// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <memory>
#include <icomp/CPackageStaticInfo.h>


class QDataStream;


namespace icomp
{


class CCachedComponentStaticInfo;
class CCachedAttributeStaticInfo;
class CCachedElementStaticInfo;


/**
	Provides file-based binary caching of package meta-information.
	This cache allows skipping DLL loading when only component
	meta-information (descriptions, keywords, interfaces, attributes) is needed.

	Uses QDataStream binary format for fast serialization/deserialization.
	The cache is invalidated when the DLL file's timestamp or size changes.
	A fast filesystem-based pre-check (modification time comparison) is used
	before opening the cache file for full validation.
*/
class CPackageMetaInfoCache
{
public:
	/**
		Load package meta-info from cache if the cache is valid.
		Combines validation and loading in a single pass to avoid
		opening and parsing the cache file twice.
		Uses a fast filesystem timestamp pre-check before reading cache contents.
		Returns a newly created CPackageStaticInfo with CCachedComponentStaticInfo instances,
		or NULL if the cache is invalid or loading fails.
		The caller takes ownership of the returned object.
	*/
	static CPackageStaticInfo* LoadFromCacheIfValid(const QString& packageFilePath, const QString& cacheFilePath);

	/**
		Save package meta-info to a binary cache file.
		Serializes the package and its embedded components using QDataStream.
		Uses atomic write (temp file + rename) for safety.
	*/
	static bool SaveToCache(const QString& cacheFilePath, const QString& packageFilePath, const CPackageStaticInfo* packageInfo);

	/**
		Compute the cache file path for a given package file.
		If cacheDir is empty, the cache file is placed in a .acf.cache
		subdirectory next to the package file.
	*/
	static QString GetCacheFilePath(const QString& packageFilePath, const QString& cacheDir = QString());

private:
	static void SerializeComponent(QDataStream& stream, const IComponentStaticInfo* componentInfo);
	static void SerializeAttribute(QDataStream& stream, const IAttributeStaticInfo* attributeInfo);
	static void SerializeSubelement(QDataStream& stream, const IElementStaticInfo* subelementInfo);

	static CCachedComponentStaticInfo* DeserializeComponent(QDataStream& stream);
	static CCachedAttributeStaticInfo* DeserializeAttribute(QDataStream& stream, const QByteArray& attributeId);
	static CCachedElementStaticInfo* DeserializeSubelement(QDataStream& stream);
};


} // namespace icomp


