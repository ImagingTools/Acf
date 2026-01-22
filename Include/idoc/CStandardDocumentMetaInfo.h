// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include <iser/ISerializable.h>
#include <idoc/IDocumentMetaInfo.h>


namespace idoc
{


/**
	Implementation of the basic document's meta information.
	
	Provides a standard implementation of IDocumentMetaInfo that stores metadata
	in a map structure. Supports all standard metadata types defined in
	IDocumentMetaInfo::MetaInfoType and custom user-defined types.
	
	The implementation is serializable, allowing metadata to be saved and loaded
	along with document content.
	
	\par Supported Standard Metadata
	- MIT_TITLE - Document title
	- MIT_AUTHOR - Document author name
	- MIT_CREATOR - Tool used to create the document
	- MIT_DESCRIPTION - Document description
	- MIT_CREATION_TIME - When document was created (QDateTime)
	- MIT_MODIFICATION_TIME - When document was last modified (QDateTime)
	- MIT_CREATOR_VERSION - Version of creation tool
	- MIT_DOCUMENT_VERSION - Document format version
	- MIT_CONTENT_CHECKSUM - Checksum of document data
	
	\par Usage Example
	\code
	// Create meta info instance
	CStandardDocumentMetaInfo metaInfo;
	
	// Set standard metadata
	metaInfo.SetMetaInfo(idoc::IDocumentMetaInfo::MIT_TITLE, "My Document");
	metaInfo.SetMetaInfo(idoc::IDocumentMetaInfo::MIT_AUTHOR, "John Doe");
	metaInfo.SetMetaInfo(idoc::IDocumentMetaInfo::MIT_CREATION_TIME, 
		QDateTime::currentDateTime());
	
	// Add custom metadata
	const int MIT_PROJECT = idoc::IDocumentMetaInfo::MIT_USER + 1;
	metaInfo.SetMetaInfo(MIT_PROJECT, "PROJECT-123");
	
	// Read metadata
	QString title = metaInfo.GetMetaInfo(
		idoc::IDocumentMetaInfo::MIT_TITLE).toString();
	
	// List all metadata
	idoc::IDocumentMetaInfo::MetaInfoTypes types = metaInfo.GetMetaInfoTypes();
	for (int type : types) {
		QString name = metaInfo.GetMetaInfoName(type);
		QVariant value = metaInfo.GetMetaInfo(type);
		qDebug() << name << "=" << value;
	}
	
	// Serialize metadata
	iser::CXmlArchive archive;
	if (archive.OpenForWrite("metadata.xml")) {
		metaInfo.Serialize(archive);
		archive.Close();
	}
	\endcode
	
	\sa IDocumentMetaInfo, IMultiPageDocument
	\ingroup DocumentBasedFramework
*/
class CStandardDocumentMetaInfo:
			virtual public IDocumentMetaInfo,
			virtual public iser::ISerializable
{
public:
	// reimplemented (idoc::IDocumentMetaInfo)
	virtual MetaInfoTypes GetMetaInfoTypes(bool allowReadOnly = true) const override;
	virtual QVariant GetMetaInfo(int metaInfoType) const override;
	virtual bool SetMetaInfo(int metaInfoType, const QVariant& metaInfo) override;
	virtual void RemoveMetaInfo(int metaInfoType) override;
	virtual QByteArray GetMetaInfoId(int metaInfoType) const override;
	virtual QString GetMetaInfoName(int metaInfoType) const override;
	virtual QString GetMetaInfoDescription(int metaInfoType) const override;
	virtual bool IsMetaInfoWritable(int metaInfoType) const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual bool IsEqual(const IChangeable& object) const override;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual istd::TUniqueInterfacePtr<istd::IChangeable> CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS) override;

private:
	typedef QMap<int, QVariant> MetaInfoMap;
	MetaInfoMap m_infosMap;
};


} // namespace idoc




