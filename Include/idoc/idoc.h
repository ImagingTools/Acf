// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


/**
	Contains the system independent basic implementations of Document/View design pattern.
	
	The idoc namespace provides the core infrastructure for building document-based applications
	using the Model-View-Controller (MVC) pattern. It includes interfaces and base implementations
	for managing documents, views, persistence, undo/redo functionality, and document metadata.
	
	\par Key Concepts
	
	\par Documents
	Documents represent the application's data model. The framework provides several document types:
	- ITextDocument - Simple text documents
	- IMultiPageDocument - Multi-page structured documents
	- Custom document types can be created by implementing appropriate interfaces
	
	\par Views
	Views display document content to the user. Multiple views can display the same document,
	and different view types can provide different representations of the same data.
	
	\par Document Templates
	Templates (IDocumentTemplate) act as factories for creating documents and views. They define:
	- Which document types are supported
	- How to create document instances
	- How to create view instances for documents
	- File persistence mechanisms (loading/saving)
	- Undo manager creation
	
	\par Document Manager
	The document manager (IDocumentManager) is the central orchestrator that:
	- Manages the lifecycle of all open documents
	- Creates and manages views for documents
	- Handles file operations (new, open, save, close)
	- Tracks the active view/document
	- Manages undo/redo per document
	- Provides recent files tracking
	
	\par Architecture Overview
	\code
	                    IDocumentManager
	                          |
	                          |-- Manages documents and views
	                          |-- Uses IDocumentTemplate for creation
	                          |
	              +-----------+-----------+
	              |                       |
	         IDocumentTemplate      Document instances
	              |                       |
	              |                       +-- ITextDocument
	     +--------+--------+              +-- IMultiPageDocument
	     |                 |              +-- Custom documents
	Creates:         Creates:
	Documents        Views           Each with optional:
	                                 - IUndoManager
	                                 - IDocumentMetaInfo
	                                 - IFilePersistence
	\endcode
	
	\par Example: Simple Text Document Application
	\code
	// 1. Define document template in configuration
	CSingleDocumentTemplateComp {
		DocumentTypeId = "TextDocument"
		DocumentTypeName = "Text Document"
		DocumentFactory = "TextDocumentFactory"
		ViewFactory = "TextViewFactory"
		DocumentLoader = "TextFileLoader"
		UndoManager = "SerializedUndoManager"
	}
	
	// 2. Use document manager to create documents
	idoc::IDocumentManager* docMgr = GetDocumentManager();
	QByteArray docType = "TextDocument";
	
	// Create new document
	docMgr->InsertNewDocument(docType, true);
	
	// Open existing document
	QString path = "/path/to/file.txt";
	docMgr->OpenDocument(&docType, &path, true);
	
	// Save active document
	docMgr->SaveDocument();
	
	// Get active view and document
	istd::IPolymorphic* view = docMgr->GetActiveView();
	istd::IChangeable* doc = docMgr->GetDocumentFromView(*view);
	
	// Use undo/redo
	idoc::IUndoManager* undoMgr = docMgr->GetUndoManagerForDocument(doc);
	if (undoMgr && undoMgr->GetAvailableUndoSteps() > 0) {
		undoMgr->DoUndo();
	}
	\endcode
	
	\par See Also
	- iqtdoc namespace for Qt-specific implementations
	- ifile namespace for file persistence
	- iser namespace for serialization
	
	\ingroup DocumentBasedFramework
*/
namespace idoc
{

} // namespace idoc



