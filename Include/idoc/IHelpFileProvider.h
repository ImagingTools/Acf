// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <idoc/IHelpInfoProvider.h>


namespace idoc
{


/**
	Allow to show help document to specified object or text.
	
	Provides file path resolution for help content. This interface extends
	IHelpInfoProvider to add the ability to map help contexts to actual file paths.
	
	Help file providers typically:
	- Map context strings to HTML files
	- Resolve object types to documentation pages
	- Handle different help file organizations (Doxygen, static HTML, etc.)
	
	\par Usage Example
	\code
	idoc::IHelpFileProvider* helpProvider = GetHelpFileProvider();
	
	// Get help file path for a context
	QString contextText = "IDocumentManager";
	QString helpPath = helpProvider->GetHelpFilePath(contextText, nullptr);
	if (!helpPath.isEmpty()) {
		qDebug() << "Help file:" << helpPath;
		// Load and display the help file
	}
	
	// Get help for an object type
	istd::IPolymorphic* obj = GetDocument();
	helpPath = helpProvider->GetHelpFilePath("", obj);
	\endcode
	
	\sa IHelpInfoProvider, IHelpViewer, CStaticHelpFileProviderComp, CDoxygenHelpFileProviderComp
	\ingroup DocumentBasedFramework
*/
class IHelpFileProvider: virtual public IHelpInfoProvider
{
public:
	/**
		Get access to help file path for specified help information.
		\param	contextText			Text describing the help context.
		\param	contextObjectPtr	Optional pointer to object needing help.
		\return	Full file path to help document (typically HTML), or empty string if no help file found.
		
		The returned path can be used to load and display the help content in a browser or viewer.
	*/
	virtual QString GetHelpFilePath(const QString& contextText, const istd::IPolymorphic* contextObjectPtr) const = 0;
};


} // namespace idoc




