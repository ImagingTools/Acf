// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// ACF includes
#include <idoc/idoc.h>

#include <iqt/iqt.h>



/**
	This package contains Qt implementations related to Document/View concept.
	
	The iqtdoc namespace builds on the idoc framework to provide Qt-specific implementations
	for creating document-based GUI applications. It includes ready-to-use components for
	main windows, workspaces, help systems, and document management.
	
	This package uses the following ACF packages:
	\li istd - Standard utilities and base classes
	\li iser - Serialization framework
	\li	icomp - Component framework
	\li	iqt - Qt integration utilities
	
	\par Key Components
	
	\par Main Window and Workspace
	- CMainWindowGuiComp - Complete main window with menu, toolbar, and file operations
	- CSingleDocumentWorkspaceGuiComp - Workspace for single document (SDI) applications
	- CMultiDocumentWorkspaceGuiComp - MDI workspace for multiple documents
	
	\par Help System
	- CHtmlHelpGuiComp - HTML-based help viewer component
	- CHtmlHelpViewerComp - Help viewer implementation
	- CStaticHelpFileProviderComp - Provides static HTML help files
	- CDoxygenHelpFileProviderComp - Integrates Doxygen-generated documentation
	
	\par Document Management
	- CSingletonDocApplicationComp - Single instance document application
	- CExtendedDocumentTemplateComp - Extended template with additional features
	- TQtDocumentManagerWrap - Qt-specific document manager wrapper
	
	\par Commands and UI
	- CUndoManagerCommandsProviderComp - Provides undo/redo commands for UI
	- CDocumentImportExportCommandComp - Import/export functionality
	- CExternalOpenDocumentCommandComp - Open documents in external applications
	- CDocumentMetaInfoEditorComp - Edit document metadata
	
	\par Building a Qt Document Application
	
	The typical structure of a Qt document-based application using iqtdoc:
	
	\code
	Application
	    |
	    +-- CMainWindowGuiComp (main window)
	         |
	         +-- CSingleDocumentWorkspaceGuiComp (or Multi)
	         |    |
	         |    +-- Document views
	         |
	         +-- IDocumentManager
	              |
	              +-- IDocumentTemplate
	                   |
	                   +-- Documents
	\endcode
	
	\par Example: Single Document Application
	\code
	// Configuration for a simple text editor
	CSingletonDocApplicationComp {
		// Main window with standard menus and toolbars
		MainWindow = CMainWindowGuiComp {
			ApplicationInfo = ApplicationInfo
			DocumentManager = DocumentManager
			
			// Single document workspace
			Workspace = CSingleDocumentWorkspaceGuiComp {
				DocumentManager = DocumentManager
			}
		}
		
		// Document manager
		DocumentManager = CSingleDocumentManagerBase {
			Template = TextDocumentTemplate
		}
		
		// Document template
		TextDocumentTemplate = CSingleDocumentTemplateComp {
			DocumentTypeId = "TextDocument"
			DocumentTypeName = "Text Document"
			DocumentFactory = CTextDocumentComp
			ViewFactory = CTextEditorGuiComp
			DocumentLoader = CTextFileLoaderComp
			UndoManager = CSerializedUndoManagerComp
		}
		
		// Optional: Help system
		HelpViewer = CHtmlHelpGuiComp {
			HelpFileProvider = CDoxygenHelpFileProviderComp {
				HelpDirectory = "doc/html"
			}
		}
	}
	\endcode
	
	\par Example: Multi-Document Application (MDI)
	\code
	// Configuration for MDI application
	CMultiDocumentWorkspaceGuiComp {
		DocumentManager = DocumentManager
		
		// MDI-specific settings
		ViewMode = "Tabbed"  // or "Windowed"
		IsViewCloseEnabled = true
	}
	
	// Use multi-document manager
	CMultiDocumentManagerBase {
		Template = CompositeTemplate
	}
	
	// Support multiple document types
	CompositeTemplate = CCompositeDocumentTemplateComp {
		Templates = [
			TextDocumentTemplate,
			ImageDocumentTemplate,
			// ... more templates
		]
	}
	\endcode
	
	\par Help System Integration
	\code
	// Get help viewer
	idoc::IHelpViewer* helpViewer = GetHelpViewer();
	
	// Show help for a specific context
	QString contextText = "MainWindow";
	helpViewer->ShowHelp(contextText, nullptr);
	
	// Show help for an object
	istd::IPolymorphic* obj = GetCurrentObject();
	helpViewer->ShowHelp("", obj);
	
	// Check help quality before showing
	double quality = helpViewer->GetHelpQuality(contextText, obj);
	if (quality > 0.5) {
		// Good help available
		helpViewer->ShowHelp(contextText, obj);
	}
	\endcode
	
	\par Recent Files
	The main window automatically maintains a list of recently opened files:
	\code
	// Recent files are automatically managed by CMainWindowGuiComp
	// Configuration:
	CMainWindowGuiComp {
		MaxRecentFiles = 10  // Maximum number of recent files to remember
	}
	
	// Recent files appear in File menu automatically
	\endcode
	
	\par Undo/Redo UI Integration
	\code
	// Undo/redo commands are automatically integrated
	// into the Edit menu by CMainWindowGuiComp
	
	// Provide undo manager commands to UI
	CUndoManagerCommandsProviderComp {
		DocumentManager = DocumentManager
	}
	
	// Commands will show descriptions from undo manager
	\endcode
	
	\sa idoc namespace for core document/view interfaces
	\ingroup DocumentBasedFramework
*/
namespace iqtdoc
{

} // namespace iqtdoc




