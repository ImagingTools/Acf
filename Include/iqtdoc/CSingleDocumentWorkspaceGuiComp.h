// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

// ACF includes
#include <ibase/ICommandsProvider.h>
#include <idoc/IDocumentTemplate.h>
#include <idoc/CSingleDocumentManagerBase.h>
#include <iqtgui/TGuiComponentBase.h>
#include <iqtgui/TRestorableGuiWrap.h>
#include <iqtgui/CHierarchicalCommand.h>
#include <iqtdoc/TQtDocumentManagerWrap.h>


namespace iqtdoc
{


/**
	Qt-based workspace implementation of a single document manager.
	
	This component provides a workspace widget for Single Document Interface (SDI)
	applications. It manages a single document at a time, creating and displaying
	views within the workspace. When a new document is opened, it replaces the
	current document.
	
	SDI applications typically have one main window with one document, similar to:
	- Notepad
	- Simple text editors
	- Image viewers with one image at a time
	
	\par Component Attributes
	- \b DefaultDocumentTitle - Title for untitled documents (default: "<no name>")
	- \b RememberOpenDocumentOnExit - Restore last document on restart (default: false)
	
	\par Component References
	- \b DocumentTemplate - Document template for creating documents (required)
	
	\par Configuration Example
	\code
	CSingleDocumentWorkspaceGuiComp {
		DefaultDocumentTitle = "Untitled"
		RememberOpenDocumentOnExit = true
		DocumentTemplate = MyDocumentTemplate
	}
	\endcode
	
	\par Usage in Main Window
	\code
	CMainWindowGuiComp {
		ApplicationInfo = AppInfo
		DocumentManager = DocumentManager
		
		// Add workspace as child component
		Workspace = CSingleDocumentWorkspaceGuiComp {
			DocumentTemplate = TextDocumentTemplate
			DefaultDocumentTitle = "New Document"
			RememberOpenDocumentOnExit = true
		}
	}
	\endcode
	
	\par Features
	- Automatic view management (creates and destroys views as needed)
	- Window title updates with document name
	- Optional document persistence between sessions
	- Clean workspace for single-document workflows
	- Integrated with ACF GUI restoration system
	
	\par Typical Workflow
	1. User opens document via File > Open
	2. Workspace creates view for document
	3. View is displayed in workspace widget
	4. Window title updates to show document name
	5. User works with document
	6. On File > Open (another document), previous document is closed (with save prompt if dirty)
	7. New document and view are created
	
	\sa CMainWindowGuiComp, CSingleDocumentManagerBase, CMultiDocumentWorkspaceGuiComp
	\ingroup DocumentBasedFramework
*/
class CSingleDocumentWorkspaceGuiComp:
			public iqtdoc::TQtDocumentManagerWrap<
						idoc::CSingleDocumentManagerBase,
						iqtgui::TRestorableGuiWrap<
									iqtgui::TGuiComponentBase<QWidget> > >
{
	Q_OBJECT

public:
	typedef iqtdoc::TQtDocumentManagerWrap<
						idoc::CSingleDocumentManagerBase,
						iqtgui::TRestorableGuiWrap<
									iqtgui::TGuiComponentBase<QWidget> > > BaseClass;

	I_BEGIN_COMPONENT(CSingleDocumentWorkspaceGuiComp);
		I_REGISTER_INTERFACE(idoc::IDocumentManager);
		I_ASSIGN(m_defaultDocumentTitleAttrPtr, "DefaultDocumentTitle", "Name of the untitled document", true, "<no name>");
		I_ASSIGN(m_documentTemplateCompPtr, "DocumentTemplate", "Document template", true, "DocumentTemplate");
		I_ASSIGN(m_rememberOpenDocumentParamPtr, "RememberOpenDocumentOnExit", "If enabled, restores open document from previous session", false, "RememberOpenDocumentOnExit");
	I_END_COMPONENT;

	enum GroupId
	{
		GI_WINDOW = 0x300,
		GI_DOCUMENT,
		GI_VIEW
	};

	CSingleDocumentWorkspaceGuiComp();

	// reimplemented (idoc::IDocumentManager)
	virtual void SetActiveView(istd::IPolymorphic* viewPtr) override;

	// reimplemented (iqtgui::IGuiObject)
	virtual void OnTryClose(bool* ignoredPtr = NULL) override;

protected:
	/**
		Update titles of views or all views of specified document.
	*/
	virtual void UpdateTitle();

	/**
		Generate the title of the current document.
	*/
	virtual QString GenerateDocumentTitle() const;

	// reimplemented (idoc::CSingleDocumentManagerBase)
	virtual QString GetOpenFilePath(const QByteArray* documentTypeIdPtr = NULL) const override;
	virtual void OnViewRegistered(istd::IPolymorphic* viewPtr) override;
	virtual void OnViewRemoved(istd::IPolymorphic* viewPtr) override;
	virtual void OnDocumentRegistered() override;
	virtual void OnDocumentClosed() override;
	virtual bool QueryDocumentSave(bool* ignoredPtr) override;

	// reimplemented (iqt:CGuiComponentBase)
	virtual void OnGuiCreated() override;
	virtual void OnGuiDestroyed() override;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

	// reimplemented (istd:IChangeable)
	virtual void OnEndChanges(const ChangeSet& changeSet) override;

	// reimplemented (TRestorableGuiWrap)
	virtual void OnRestoreSettings(const QSettings& settings) override;

protected:
	I_TEXTATTR(m_defaultDocumentTitleAttrPtr);
	I_REF(idoc::IDocumentTemplate, m_documentTemplateCompPtr);
	I_REF(iprm::IEnableableParam, m_rememberOpenDocumentParamPtr);

	iqtgui::IGuiObject* m_lastViewPtr;

	QString m_organizationName;
	QString m_applicationName;
};


} // namespace iqtdoc


