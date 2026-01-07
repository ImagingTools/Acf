#ifndef idoc_IHelpViewer_included
#define idoc_IHelpViewer_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <istd/IPolymorphic.h>
#include <idoc/IHelpInfoProvider.h>


namespace idoc
{


/**
	Allow to show help document to specified object or text.
	
	Extends IHelpInfoProvider to provide actual help display functionality.
	This is the main interface for help viewer components that can show help
	content to the user.
	
	\par Usage Example
	\code
	idoc::IHelpViewer* helpViewer = GetHelpViewer();
	
	// Show help for a specific topic
	helpViewer->ShowHelp("MainWindow", nullptr);
	
	// Show help for an object
	istd::IPolymorphic* obj = GetCurrentWidget();
	helpViewer->ShowHelp("", obj);
	
	// Check quality before showing
	if (helpViewer->GetHelpQuality("UndoRedo", nullptr) > 0.5) {
		helpViewer->ShowHelp("UndoRedo", nullptr);
	}
	\endcode
	
	\sa IHelpInfoProvider, IHelpFileProvider, CHtmlHelpGuiComp
	\ingroup DocumentBasedFramework
*/
class IHelpViewer: virtual public IHelpInfoProvider
{
public:
	/**
		Show some help information for specified context.
		\param	contextText			Text describing the help context (e.g., "FileMenu", "UndoCommand").
		\param	contextObjectPtr	Optional pointer to object needing help.
		
		This method displays help content in an appropriate viewer (browser, dialog, etc.).
		The implementation determines the best way to present the help based on the context.
	*/
	virtual void ShowHelp(const QString& contextText, const istd::IPolymorphic* contextObjectPtr) = 0;
};


} // namespace idoc


#endif // !idoc_IHelpViewer_included


