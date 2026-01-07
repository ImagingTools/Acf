#pragma once


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <istd/IChangeable.h>
#include <ifile/IFilePersistence.h>
#include <imod/IObserver.h>
#include <ibase/ICommand.h>
#include <idoc/IUndoManager.h>
#include <idoc/IDocumentTypesInfo.h>


namespace idoc
{


/**
	Common interface for a document template.
	
	Document template provides all necessary functionality for creating and controlling
	the data model of a document, its persistence, and its representation (view).
	A template acts as a factory for documents and views, and defines the relationship
	between document types, their file formats, and their visual representations.
	
	The template is responsible for:
	- Creating document instances of specific types
	- Creating view instances to display documents
	- Providing file persistence (loading/saving)
	- Creating undo managers for documents
	- Managing document type metadata
	
	Templates can be single-type (supporting one document type) or composite
	(supporting multiple document types).
	
	\par Usage Example
	\code
	// Get a document template
	idoc::IDocumentTemplate* tmpl = GetDocumentTemplate();
	
	// Get available document types
	idoc::IDocumentTemplate::Ids docTypes = tmpl->GetDocumentTypeIds();
	for (const QByteArray& typeId : docTypes) {
		QString typeName = tmpl->GetDocumentTypeName(typeId);
		qDebug() << "Available type:" << typeName << "(" << typeId << ")";
	}
	
	// Create a new document
	QByteArray docTypeId = "TextDocument";
	istd::IChangeableUniquePtr docPtr = tmpl->CreateDocument(docTypeId, true);
	
	// Create a view for the document
	istd::IPolymorphic* view = tmpl->CreateView(docTypeId, docPtr.Get()).Release();
	
	// Get file loader for persistence
	ifile::IFilePersistence* loader = tmpl->GetFileLoader(docTypeId);
	if (loader) {
		// Use loader to save/load the document
		loader->SaveToFile(docPtr.Get(), "/path/to/file.txt");
	}
	\endcode
	
	\sa IDocumentManager, CSingleDocumentTemplateComp, CCompositeDocumentTemplateComp
	\ingroup DocumentBasedFramework
*/
class IDocumentTemplate: virtual public IDocumentTypesInfo
{
public:
	/**
		Type definitions for view smart pointers.
	*/
	typedef istd::TUniqueInterfacePtr<istd::IPolymorphic> ViewUniquePtr;
	typedef istd::TSharedInterfacePtr<istd::IPolymorphic> ViewSharedPtr;

	/**
		Return supported view type IDs for specified document type.
		\param	documentTypeId	ID of document type.
		\return	Vector of view type IDs that can display this document type.
		
		A single document type may support multiple view types (e.g., text view, hex view, preview).
	*/
	virtual Ids GetViewTypeIds(const QByteArray& documentTypeId) const = 0;

	/**
		Get human readable name of some view type ID.
		\param	documentTypeId	ID of document type.
		\param	viewTypeId		ID of view type.
		\return	Localized human-readable name for the view type (e.g., "Text View", "Hex View").
	*/
	virtual QString GetViewTypeName(
				const QByteArray& documentTypeId,
				const QByteArray& viewTypeId) const = 0;

	/**
		Get file loader/saver for specified document ID.
		\param	documentTypeId	Type ID of document that will be loaded/saved.
		\return	Pointer to file persistence object that handles I/O operations, or NULL if not supported.
		
		The file persistence object handles reading and writing documents to/from files.
	*/
	virtual ifile::IFilePersistence* GetFileLoader(const QByteArray& documentTypeId) const = 0;

	/**
		Creates a document instance for document type \c documentTypeId.
		\param	documentTypeId	ID of document type, can be empty for default document.
								If document type is undefined (empty), this field will be used to return ID of used type.
		\param	initialize		If true, document should be filled with initial data (operation new),
								otherwise initialization is not needed (e.g., for document loading - filling of data will be done by loader).
		\param	beQuiet			If true, no user interaction dialogs will be shown.
		\param	ignoredFlagPtr	Optional output flag indicating that the user canceled this operation.
		\return	Unique pointer to the created document, or nullptr if creation failed.
		
		\par Example
		\code
		QByteArray docType = "TextDocument";
		istd::IChangeableUniquePtr doc = tmpl->CreateDocument(docType, true);
		if (doc) {
			// Document created successfully
		}
		\endcode
	*/
	virtual istd::IChangeableUniquePtr CreateDocument(
				QByteArray& documentTypeId,
				bool initialize = true,
				bool beQuiet = false,
				bool* ignoredFlagPtr = NULL) const = 0;

	/**
		Creates a view instance for document \c document of type \c viewTypeId.
		\param	documentTypeId	ID of document type.
		\param	documentPtr		Pointer to document object that this view will display.
		\param	viewTypeId		Optional ID specifying view type if more view types are supported.
								If empty or not provided, the first/default view type for this document type is used.
								Use GetViewTypeIds() to discover available view types for a document type.
		\return	Unique pointer to the created view, or nullptr if creation failed.
		
		The view is responsible for displaying the document's content to the user.
		Some document types support multiple view types (e.g., text view, hex view, preview).
		
		\par Example
		\code
		// Create default view
		istd::IPolymorphic* view = tmpl->CreateView(docTypeId, doc.Get()).Release();
		
		// Create specific view type
		QByteArray viewType = "HexView";
		istd::IPolymorphic* hexView = tmpl->CreateView(docTypeId, doc.Get(), viewType).Release();
		\endcode
	*/
	virtual ViewUniquePtr CreateView(
				const QByteArray& documentTypeId,
				istd::IChangeable* documentPtr,
				const QByteArray& viewTypeId = QByteArray()) const = 0;

	/**
		Creates an undo manager for a given document \c documentPtr.
		\param	documentTypeId	ID of document type.
		\param	documentPtr		Pointer to document object that will use this undo manager.
		\return	Unique pointer to created instance of undo manager, or nullptr if creating of undo manager was impossible.
		
		The undo manager tracks changes to the document and allows them to be undone/redone.
		Not all document types support undo functionality.
	*/
	virtual idoc::IUndoManagerUniquePtr CreateUndoManager(const QByteArray& documentTypeId, istd::IChangeable* documentPtr) const = 0;
};


} // namespace idoc




