// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QMap>

// Qt includes
#include <QtCore/QString>

// ACF includes
#include <istd/IChangeable.h>
#include <idoc/IUndoManager.h>
#include <idoc/IDocumentTypesInfo.h>


namespace ibase
{
	class IProgressManager;
}


namespace idoc
{


/**
	Provide set of user actions needed to manage documents in MVC (Model View Controller) concept.
	
	The document manager is the central component in the ACF document/view architecture.
	It manages the lifecycle of documents and their associated views, handles file operations
	(new, open, save, close), and maintains the document state.
	
	Key responsibilities:
	- Document lifecycle management (create, open, save, close)
	- View management and activation
	- Undo/redo support per document
	- File persistence integration
	- Recent files tracking
	- Document type identification
	
	The manager supports both single and multi-document interfaces (SDI/MDI).
	For SDI applications, typically only one document is active at a time.
	For MDI applications, multiple documents can be open simultaneously with one being active.
	
	\par Usage Example
	\code
	// Get the document manager from application
	idoc::IDocumentManager* docMgr = GetDocumentManager();
	
	// Create a new document
	QByteArray docTypeId = "TextDocument";
	if (docMgr->InsertNewDocument(docTypeId, true)) {
		// Document created successfully with a view
	}
	
	// Open an existing document
	QString filePath = "/path/to/document.txt";
	if (docMgr->OpenDocument(&docTypeId, &filePath, true)) {
		// Document opened and view created
	}
	
	// Save the active document
	if (docMgr->SaveDocument()) {
		// Document saved successfully
	}
	
	// Get information about all open documents
	int count = docMgr->GetDocumentsCount();
	for (int i = 0; i < count; ++i) {
		idoc::IDocumentManager::DocumentInfo info;
		istd::IChangeable& doc = docMgr->GetDocumentFromIndex(i, &info);
		qDebug() << "Document:" << info.filePath << "Type:" << info.documentTypeId << "Dirty:" << info.isDirty;
	}
	
	// Close the active document
	docMgr->CloseDocument(-1, false); // -1 = active document, false = ask user to save
	\endcode
	
	\sa IDocumentTemplate, IUndoManager, IDocumentMetaInfo
	\ingroup DocumentBasedFramework
*/
class IDocumentManager:
			virtual public istd::IChangeable,
			virtual public IDocumentTypesInfo
{
public:
	/**
		Possible changes in the manager data model.
		These flags are used to notify observers about specific changes in the document manager state.
	*/
	enum ChangeFlags
	{
		/**
			A document has been renamed or its file path has changed.
		*/
		CF_DOCUMENT_RENAMED = 0x834d670,
		/**
			A new document has been created.
		*/
		CF_DOCUMENT_CREATED,
		/**
			A document has been removed from the manager.
		*/
		CF_DOCUMENT_REMOVED,
		/**
			The total number of managed documents has changed.
		*/
		CF_DOCUMENT_COUNT_CHANGED,
		/**
			The active view has changed to a different view.
		*/
		CF_VIEW_ACTIVATION_CHANGED
	};

	/**
		Information about a document managed by the document manager.
	*/
	struct DocumentInfo
	{
		QString filePath;			///< Full file path of the document, empty if not saved yet
		QByteArray documentTypeId;	///< Type identifier of the document
		bool isDirty;				///< True if document has unsaved changes
	};

	/**
		Flags describing possible operations that can be performed by the document manager.
		These flags indicate which file operations are currently supported.
	*/
	enum OperationFlags
	{
		OF_FILE_NEW = 0x0001,		///< New document creation is supported
		OF_FILE_OPEN = 0x0002,		///< Opening existing documents is supported
		OF_FILE_SAVE = 0x0004,		///< Saving documents is supported
		OF_FILE_SAVE_AS = 0x0008,	///< Save As operation is supported
		OF_KNOWN_PATH = 0x0010		///< Document has a known file path
	};

	/**
		Map from file path to document type ID's.
		Used for tracking loaded files and their associated document types.
	*/
	typedef QMap<QString, QByteArray> FileToTypeMap;

	/**
		Get flags of allowed operations.
		\param	viewPtr		Optional pointer to a specific view to check operations for.
							If NULL, checks operations for the active view/document.
		\return	Bitwise OR of OperationFlags indicating which operations are currently allowed.
		
		\par Example
		\code
		int flags = docMgr->GetAllowedOperationFlags();
		if (flags & idoc::IDocumentManager::OF_FILE_NEW) {
			// New document creation is supported
		}
		if (flags & idoc::IDocumentManager::OF_FILE_SAVE) {
			// Saving is available
		}
		\endcode
	*/
	virtual int GetAllowedOperationFlags(const istd::IPolymorphic* viewPtr = NULL) const = 0;

	/**
		Return undo manager for document \c documentPtr.
		\param	documentPtr		Pointer to the document to get undo manager for.
		\return	Pointer to the undo manager associated with this document, or NULL if no undo manager exists.
		
		Each document can have its own undo manager to track and reverse changes.
		
		\par Example
		\code
		istd::IChangeable* doc = docMgr->GetDocumentFromView(*view);
		idoc::IUndoManager* undoMgr = docMgr->GetUndoManagerForDocument(doc);
		if (undoMgr && undoMgr->GetAvailableUndoSteps() > 0) {
			undoMgr->DoUndo();
		}
		\endcode
	*/
	virtual idoc::IUndoManager* GetUndoManagerForDocument(const istd::IChangeable* documentPtr) const = 0;

	/**
		Get number of opened documents.
		\return	The total count of currently open documents managed by this manager.
	*/
	virtual int GetDocumentsCount() const = 0;

	/**
		Get document at specified index.
		\param	index			Zero-based index of document (0 to GetDocumentsCount()-1).
		\param	documentInfoPtr	Optional pointer to receive additional information about the document.
		\return	Reference to the document at the specified index.
		
		\par Example
		\code
		for (int i = 0; i < docMgr->GetDocumentsCount(); ++i) {
			idoc::IDocumentManager::DocumentInfo info;
			istd::IChangeable& doc = docMgr->GetDocumentFromIndex(i, &info);
			qDebug() << "Document" << i << "path:" << info.filePath;
		}
		\endcode
	*/
	virtual istd::IChangeable& GetDocumentFromIndex(int index, DocumentInfo* documentInfoPtr = NULL) const = 0;

	/**
		Get number of view for specified document.
	*/
	virtual int GetViewsCount(int documentIndex) const = 0;

	/**
		Get single view using its and document indices.
	*/
	virtual istd::IPolymorphic* GetViewFromIndex(int documentIndex, int viewIndex) const = 0;

	/**
		Return the active document. 
		If no document is active, the function return a \c NULL
	*/
	virtual istd::IPolymorphic* GetActiveView() const = 0;

	/**
		Indicate that some view is active now.
	*/
	virtual void SetActiveView(istd::IPolymorphic* viewPtr) = 0;

	/**
		Return the document assigned to view. 
		\param	view			view object.
		\param	documentInfoPtr	optional return value describing some additional informations about document.
		\return					pointer to assigned document, or \c NULL if no document for this view exists.
	*/
	virtual istd::IChangeable* GetDocumentFromView(const istd::IPolymorphic& view, DocumentInfo* documentInfoPtr = NULL) const = 0;

	/**
		Add a new view to the document.
		\param	document		document instance
		\param	viewTypeId		ID of view type, if it will be created.
		\return					Created view object
	*/
	virtual istd::IPolymorphic* AddViewToDocument(const istd::IChangeable& document, const QByteArray& viewTypeId = QByteArray()) = 0;

	/**
		Get ID of document type managed by this object.
		\return	ID of document type or empty string if no document is found.
	*/
	virtual QByteArray GetDocumentTypeId(const istd::IChangeable& document) const = 0;

	/**
		Creates a new document with the document ID \c documentTypeId.
		\param	documentTypeId	ID of document type.
		\param	createView		if true, view will be automatically created.
		\param	viewTypeId		ID of view type, if it will be created.
		\param	newDocumentPtr	optional pointer where new created document will be returned.
		\param	beQuiet			if true, no user interaction will be done.
		\param	ignoredPtr		optional return flag indicating that creating was aborted by user.
		\return	true, if success.
	*/
	virtual bool InsertNewDocument(
				const QByteArray& documentTypeId, 
				bool createView = true, 
				const QByteArray& viewTypeId = "",
				istd::IChangeableSharedPtr* newDocumentPtr = nullptr,
				bool beQuiet = false,
				bool* ignoredPtr = NULL) = 0;

	/**
		Opens document(s) from the file list.
		\param	documentTypeIdPtr	optional ID of document type. If it is NULL, document type will be found automatically.
		\param	fileNamePtr			file name.
		\param	createView			if true, view will be automatically created.
		\param	viewTypeId			ID of view type, if it will be created.
		\param	loadedMapPtr		optional list of loaded files and its document type ID's.
		\param	beQuiet			if true, no user interaction will be done.
		\param	ignoredPtr			optional return flag indicating that opening was aborted by user.
	*/
	virtual bool OpenDocument(
				const QByteArray* documentTypeIdPtr = NULL,
				const QString* fileNamePtr = NULL,
				bool createView = true,
				const QByteArray& viewTypeId = "",
				istd::IChangeableSharedPtr* documentPtr = nullptr,
				FileToTypeMap* loadedMapPtr = NULL,
				bool beQuiet = false,
				bool* ignoredPtr = NULL,
				ibase::IProgressManager* progressManagerPtr = NULL) = 0;

	/**
		Save document.
		\param	documentIndex	optional index of document to save, if negative then current active document will be saved.
		\param	requestFileName	if true, file name will be always requested by user.
		\param	savedMapPtr		optional list of files will be extended with currently saved file.
		\param	beQuiet			if true, no user interaction will be done.
		\param	ignoredPtr		optional return flag indicating that saving was aborted by user.
	*/
	virtual bool SaveDocument(
				int documentIndex = -1,
				bool requestFileName = false,
				FileToTypeMap* savedMapPtr = NULL,
				bool beQuiet = false,
				bool* ignoredPtr = NULL,
				ibase::IProgressManager* progressManagerPtr = NULL) = 0;

	/**
		Ask user (optional) and save all dirty (changed) documents.
		\param	beQuiet			if true, no user interaction will be done.
								Typically user will be asked for saving document.
		\param	ignoredPtr		optional return flag indicating that process was aborted by user.
		\return	true if all documents were saved and are not dirty now.
	*/
	virtual bool SaveDirtyDocuments(bool beQuiet = false, bool* ignoredPtr = NULL) = 0;

	/**
		Close document and all its views.
		\param	documentIndex	optional index of document to close, if negative then current active document will be closed.
		\param	beQuiet			if true, no user interaction will be done.
								Typically user will be asked for saving document.
		\param	ignoredPtr		optional return flag indicating that closing was aborted by user.
	*/
	virtual bool CloseDocument(int documentIndex = -1, bool beQuiet = false, bool* ignoredPtr = NULL) = 0;

	/**
		Close view.
		\param	viewPtr			pointer to view should be closed or \c NULL if current active view should be closed.
		\param	beQuiet			if true, no user interaction will be done.
								Typically user will be asked for saving document.
		\param	ignoredPtr		optional return flag indicating that closing was aborted by user.
	*/
	virtual bool CloseView(istd::IPolymorphic* viewPtr = NULL, bool beQuiet = false, bool* ignoredPtr = NULL) = 0;
};


} // namespace idoc


