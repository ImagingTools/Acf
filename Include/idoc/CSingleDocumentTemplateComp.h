// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <imod/IModel.h>
#include <icomp/CComponentBase.h>
#include <ifile/IFilePersistence.h>
#include <idoc/CSingleDocumentTemplateBase.h>


namespace idoc
{


/**
	Document template component supporting only one type of the provided document.
	
	This is the standard template implementation for applications that work with a single
	document type. It integrates a document factory, view factory, file loader, and
	optional undo manager into a cohesive template.
	
	\par Component Attributes
	- \b DocumentTypeId - Unique identifier for this document type (default: "Default")
	- \b DocumentTypeName - Human-readable name shown in UI
	- \b DefaultDirectory - Default directory for file operations (default: ".")
	- \b IsNewSupported - Whether "File > New" is supported (default: false)
	- \b IsEditSupported - Whether document is editable (default: false)
	
	\par Component References
	- \b DocumentFactory - Factory for creating document instances (required)
	- \b ViewFactory - Factory for creating view instances (required)
	- \b DocumentLoader - Component handling file I/O (required)
	- \b UndoManager - Optional undo manager factory
	
	\par Configuration Example
	\code
	CSingleDocumentTemplateComp {
		DocumentTypeId = "TextDocument"
		DocumentTypeName = "Text Document"
		DefaultDirectory = "~/Documents"
		IsNewSupported = true
		IsEditSupported = true
		
		DocumentFactory = CTextDocumentComp {
			DefaultText = "Type here..."
		}
		
		ViewFactory = CTextEditorGuiComp
		
		DocumentLoader = CTextFileLoaderComp {
			FileExtensions = ["txt", "text"]
			TypeDescriptions = ["Text File", "Plain Text"]
		}
		
		UndoManager = CSerializedUndoManagerComp {
			MaxBufferSize = 50
		}
	}
	\endcode
	
	\par Usage in Single Document Application
	\code
	// Define in application configuration
	CSingleDocumentManagerBase {
		Template = MyDocumentTemplate
	}
	
	MyDocumentTemplate = CSingleDocumentTemplateComp {
		DocumentTypeId = "MyDoc"
		DocumentTypeName = "My Document"
		IsNewSupported = true
		IsEditSupported = true
		DocumentFactory = MyDocumentComp
		ViewFactory = MyViewComp
		DocumentLoader = MyLoaderComp
	}
	\endcode
	
	The template handles:
	- Creating new document instances
	- Creating views for documents
	- Providing file persistence operations
	- Creating undo managers
	- Reporting document type capabilities
	
	\sa IDocumentTemplate, CSingleDocumentManagerBase, CCompositeDocumentTemplateComp
	\ingroup DocumentBasedFramework
*/
class CSingleDocumentTemplateComp: public icomp::CComponentBase, public CSingleDocumentTemplateBase
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef CSingleDocumentTemplateBase BaseClass2;

	I_BEGIN_COMPONENT(CSingleDocumentTemplateComp);
		I_REGISTER_INTERFACE(idoc::IDocumentTemplate);
		I_ASSIGN(m_documentTypeIdAttrPtr, "DocumentTypeId", "ID of supported document", true, "Default");
		I_ASSIGN(m_documentTypeNameAttrPtr, "DocumentTypeName", "Human readable name of document", false, "");
		I_ASSIGN(m_defaultDirectoryAttrPtr, "DefaultDirectory", "Default file directory for open file dialog", true, ".");
		I_ASSIGN(m_documentCompFact, "DocumentFactory", "Document factory", true, "DocumentFactory");
		I_ASSIGN_TO(m_modelCompFact, m_documentCompFact, true);
		I_ASSIGN(m_viewCompFact, "ViewFactory", "Create of document GUI", true, "ViewFactory");
		I_ASSIGN(m_fileLoaderCompPtr, "DocumentLoader", "Provide document loading and saving", true, "DocumentLoader");
		I_ASSIGN(m_undoManagerCompFact, "UndoManager", "Undo manager providing undo functionality", false, "UndoManager");
		I_ASSIGN_TO(m_undoManagerObserverCompFact, m_undoManagerCompFact, true);
		I_ASSIGN(m_isNewSupportedAttrPtr, "IsNewSupported", "Template supports the new operation", true, false);
		I_ASSIGN(m_isEditSupportedAttrPtr, "IsEditSupported", "Template supports the edit operation", true, false);
	I_END_COMPONENT;

	// reimplemented (idoc::IDocumentTemplate)
	virtual ifile::IFilePersistence* GetFileLoader(const QByteArray& documentTypeId) const override;
	virtual istd::IChangeableUniquePtr CreateDocument(
				QByteArray& documentTypeId,
				bool initialize = true,
				bool beQuiet = false,
				bool* ignoredFlagPtr = NULL) const override;
	virtual ViewUniquePtr CreateView(
				const QByteArray& documentTypeId,
				istd::IChangeable* documentPtr,
				const QByteArray& viewTypeId = QByteArray()) const override;
	virtual idoc::IUndoManagerUniquePtr CreateUndoManager(const QByteArray& documentTypeId, istd::IChangeable* documentPtr) const override;

protected:
	/**
		Extract pointer of view interface from factorisied view component.
		This functionality is provided to allow derrived classes
		to extract implementation-specific GUI interfaces from created composited instances.
		Default is instance of interface imod::IObserver extracted, but for
		composite components it can be inaccurate for some cases.
	*/
	virtual ViewUniquePtr ExtractViewInterface(icomp::IComponentUniquePtr& componentPtr) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

private:
	I_ATTR(bool, m_isNewSupportedAttrPtr);
	I_ATTR(bool, m_isEditSupportedAttrPtr);
	I_ATTR(QByteArray, m_documentTypeIdAttrPtr);
	I_TEXTATTR(m_documentTypeNameAttrPtr);
	I_ATTR(QString, m_defaultDirectoryAttrPtr);
	I_FACT(istd::IChangeable, m_documentCompFact);
	I_FACT(imod::IModel, m_modelCompFact);
	I_FACT(imod::IObserver, m_viewCompFact);
	I_REF(ifile::IFilePersistence, m_fileLoaderCompPtr);
	I_FACT(idoc::IUndoManager, m_undoManagerCompFact);
	I_FACT(imod::IObserver, m_undoManagerObserverCompFact);
};


} // namespace idoc



