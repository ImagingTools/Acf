// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QTextBrowser>
#include <QtPrintSupport/QPrinter>
#else
#include <QtGui/QTextBrowser>
#include <QtGui/QPrinter>
#endif

// ACF includes
#include <iqtgui/TGuiComponentBase.h>
#include <idoc/IHelpViewer.h>
#include <idoc/IHelpFileProvider.h>


namespace iqtdoc
{


/**
	HTML-based help viewer component.
	
	Provides a Qt-based HTML help viewer that can display HTML documentation files.
	The component uses QTextBrowser to render HTML content and integrates with the
	ACF help system through IHelpViewer interface.
	
	This component is typically used to display:
	- Doxygen-generated documentation
	- Static HTML help files
	- Context-sensitive help
	- Application documentation
	
	\par Component References
	- \b HelpFileProvider - Component that resolves help context to file paths (required)
	
	\par Registered Interfaces
	- idoc::IHelpInfoProvider - Reports help availability
	- idoc::IHelpViewer - Displays help content
	
	\par Configuration Example
	\code
	CHtmlHelpGuiComp {
		HelpFileProvider = CDoxygenHelpFileProviderComp {
			HelpDirectory = "doc/html"
			IndexFileName = "index.html"
		}
	}
	\endcode
	
	\par Alternative Configuration with Static Help
	\code
	CHtmlHelpGuiComp {
		HelpFileProvider = CStaticHelpFileProviderComp {
			HelpDirectory = "help"
			HelpFileExtension = "html"
		}
	}
	\endcode
	
	\par Usage in Main Window
	\code
	CMainWindowGuiComp {
		// Main window will automatically use help viewer if available
		HelpViewer = CHtmlHelpGuiComp {
			HelpFileProvider = MyHelpProvider
		}
	}
	\endcode
	
	\par Programmatic Usage
	\code
	// Get help viewer instance
	idoc::IHelpViewer* helpViewer = GetHelpViewer();
	
	// Show help for a topic
	helpViewer->ShowHelp("IDocumentManager", nullptr);
	
	// Show help for current widget
	QWidget* widget = GetCurrentWidget();
	helpViewer->ShowHelp("", widget);
	
	// Check if help is available before showing
	double quality = helpViewer->GetHelpQuality("SomeClass", nullptr);
	if (quality > 0.5) {
		helpViewer->ShowHelp("SomeClass", nullptr);
	}
	\endcode
	
	The component features:
	- Automatic URL resolution through help file provider
	- HTML content rendering with QTextBrowser
	- Navigation history (back/forward)
	- Link following
	- Print support (via QTextBrowser capabilities)
	
	\sa IHelpViewer, IHelpFileProvider, CDoxygenHelpFileProviderComp, CStaticHelpFileProviderComp
	\ingroup DocumentBasedFramework
*/
class CHtmlHelpGuiComp:
			public iqtgui::TGuiComponentBase<QTextBrowser>,
			virtual public idoc::IHelpViewer
{
public:
	typedef iqtgui::TGuiComponentBase<QTextBrowser> BaseClass;

	I_BEGIN_COMPONENT(CHtmlHelpGuiComp);
		I_REGISTER_INTERFACE(idoc::IHelpInfoProvider);
		I_REGISTER_INTERFACE(idoc::IHelpViewer);
		I_ASSIGN(m_helpFileProviderCompPtr, "HelpFileProvider", "Calculate path of html document", true, "HelpFileProvider");
	I_END_COMPONENT;

	// reimplemented (idoc::IHelpInfoProvider)
	virtual double GetHelpQuality(const QString& contextText, const istd::IPolymorphic* contextObjectPtr) const override;

	// reimplemented (idoc::IHelpViewer)
	virtual void ShowHelp(const QString& contextText, const istd::IPolymorphic* contextObjectPtr) override;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated() override;
	virtual void OnGuiShown() override;

private:
	I_REF(idoc::IHelpFileProvider, m_helpFileProviderCompPtr);

	mutable QUrl m_urlToShow;
};


} // namespace iqtdoc


