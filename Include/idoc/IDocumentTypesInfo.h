#ifndef idoc_IDocumentTypesInfo_included
#define idoc_IDocumentTypesInfo_included


// Qt includes
#include <QtCore/QString>
#include <QtCore/QVector>

// ACF includes
#include <ifile/IFileTypeInfo.h>


namespace idoc
{


/**
	Provide information about different document types.
	
	This interface provides metadata about document types supported by a template or manager.
	It includes information about document type IDs, names, supported features, file types,
	and default directories.
	
	Document types define what kinds of documents can be created or opened by the application.
	Each type has a unique ID and associated metadata that describes its capabilities.
	
	\par Usage Example
	\code
	idoc::IDocumentTypesInfo* typesInfo = GetDocumentTypesInfo();
	
	// Get all supported document types
	idoc::IDocumentTypesInfo::Ids typeIds = typesInfo->GetDocumentTypeIds();
	for (const QByteArray& typeId : typeIds) {
		QString name = typesInfo->GetDocumentTypeName(typeId);
		qDebug() << "Type:" << name << "ID:" << typeId;
		
		// Check supported features
		bool canCreate = typesInfo->IsFeatureSupported(
			idoc::IDocumentTypesInfo::SF_NEW_DOCUMENT, typeId);
		bool canEdit = typesInfo->IsFeatureSupported(
			idoc::IDocumentTypesInfo::SF_EDIT_DOCUMENT, typeId);
		
		qDebug() << "  Can create:" << canCreate << "Can edit:" << canEdit;
		
		// Get file type info
		ifile::IFileTypeInfo* fileInfo = typesInfo->GetDocumentFileTypeInfo(typeId);
		if (fileInfo) {
			QString filter = fileInfo->GetFileDialogFilter();
			qDebug() << "  File filter:" << filter;
		}
	}
	
	// Get document types for a specific file
	QString filePath = "/path/to/document.txt";
	idoc::IDocumentTypesInfo::Ids compatibleTypes = 
		typesInfo->GetDocumentTypeIdsForFile(filePath);
	for (const QByteArray& typeId : compatibleTypes) {
		qDebug() << "File compatible with:" << typesInfo->GetDocumentTypeName(typeId);
	}
	
	// Get default directory for a document type
	QString defaultDir = typesInfo->GetDefaultDirectory("", &typeIds[0]);
	\endcode
	
	\sa IDocumentTemplate, IDocumentManager
	\ingroup DocumentBasedFramework
*/
class IDocumentTypesInfo: virtual public istd::IPolymorphic
{
public:
	/**
		Type definition for document type ID collections.
	*/
	typedef QVector<QByteArray> Ids;

	/**
		Enumeration for supported types of operations with documents.
	*/
	enum SupportedFeatures
	{
		/**
			A new document can be created (File > New).
		*/
		SF_NEW_DOCUMENT = 0x0001,

		/**
			A document is editable (content can be modified).
		*/
		SF_EDIT_DOCUMENT = 0x0010,

		/**
			All features are supported.
		*/
		SF_DEFAULT = 0xffff
	};

	/**
		Return \c true, if the feature(s) is supported by this document template.
		\param	featureFlags	Bitwise OR of SupportedFeatures flags to check.
		\param	documentTypeId	ID of document type to check.
		\return	True if all specified features are supported for this document type.
	*/
	virtual bool IsFeatureSupported(int featureFlags, const QByteArray& documentTypeId) const = 0;

	/**
		Get list of supported document type IDs.
		\return	Vector of document type IDs supported by this template/manager.
	*/
	virtual Ids GetDocumentTypeIds() const = 0;

	/**
		Get human readable name of some document type ID.
		\param	documentTypeId	ID of document type as returned by \c GetDocumentTypeIds().
		\return	Localized, user-friendly name of the document type (e.g., "Text Document", "Image File").
	*/
	virtual QString GetDocumentTypeName(const QByteArray& documentTypeId) const = 0;

	/**
		Get file type information object for some selected document type.
		\param	documentTypeId	ID of document type as returned by \c GetDocumentTypeIds().
		\return	Pointer to file type info providing file extensions, filters, etc., or NULL if not available.
	*/
	virtual ifile::IFileTypeInfo* GetDocumentFileTypeInfo(const QByteArray& documentTypeId) const = 0;

	/**
		Get list of supported document IDs that can handle the specified file.
		\param	filePath	Full path to file to check.
		\return	Vector of document type IDs that can open/handle this file.
		
		This method determines which document types are compatible with a given file
		based on file extension, content analysis, or other criteria.
	*/
	virtual Ids GetDocumentTypeIdsForFile(const QString& filePath) const = 0;

	/**
		Return default directory for specified document type.
		\param	suggestedDir		Template directory suggested by user or application.
		\param	documentTypeIdPtr	Optional pointer to ID of document type.
		\return	Resolved default directory path to use for file dialogs.
		
		If suggestedDir is provided and valid, it may be used. Otherwise, the type-specific
		default directory is returned.
	*/
	virtual QString GetDefaultDirectory(const QString& sugestedDir = "", const QByteArray* documentTypeIdPtr = NULL) const = 0;
};


} // namespace idoc


#endif // !idoc_IDocumentTypesInfo_included


