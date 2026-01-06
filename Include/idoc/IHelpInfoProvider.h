#ifndef idoc_IHelpInfoProvider_included
#define idoc_IHelpInfoProvider_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <istd/IPolymorphic.h>


namespace idoc
{


/**
	Allow to show help document to specified object or text.
	
	Provides a quality metric for help availability. This interface is the base
	for help system components and allows checking if help is available for a
	given context before attempting to display it.
	
	The quality value (0.0 to 1.0) indicates how relevant and specific the help
	content is for the given context:
	- 0.0 = No help available
	- 0.5 = Generic or partial help available
	- 1.0 = Specific, high-quality contextual help available
	
	\par Usage Example
	\code
	idoc::IHelpInfoProvider* helpProvider = GetHelpProvider();
	
	// Check help quality for a text context
	QString context = "MainWindow";
	double quality = helpProvider->GetHelpQuality(context, nullptr);
	if (quality > 0.0) {
		qDebug() << "Help available with quality:" << quality;
	}
	
	// Check help for an object
	istd::IPolymorphic* obj = GetCurrentObject();
	quality = helpProvider->GetHelpQuality("", obj);
	
	// Only show help if good quality is available
	if (quality >= 0.7) {
		// High quality help available, display it
	}
	\endcode
	
	\sa IHelpViewer, IHelpFileProvider
	\ingroup DocumentBasedFramework
*/
class IHelpInfoProvider: virtual public istd::IPolymorphic
{
public:
	/**
		Get quality of help information for specified context and/or object.
		\param	contextText			Text describing the help context (e.g., class name, topic).
		\param	contextObjectPtr	Optional pointer to object needing help.
		\return	Quality of help represented as value in range [0, 1].
				- 0 means there is no usable help
				- 1 means good context-specific help is available
				- Values in between indicate partial or generic help availability
		
		Either contextText or contextObjectPtr can be provided, or both.
		The help provider uses these to determine if relevant help exists.
	*/
	virtual double GetHelpQuality(const QString& contextText, const istd::IPolymorphic* contextObjectPtr) const = 0;
};


} // namespace idoc


#endif // !idoc_IHelpInfoProvider_included


