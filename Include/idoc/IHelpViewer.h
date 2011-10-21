#ifndef idoc_IHelpViewer_included
#define idoc_IHelpViewer_included


// ACF includes
#include "istd/IPolymorphic.h"
#include "istd/CString.h"

#include "idoc/IHelpInfoProvider.h"


namespace idoc
{


/**
	Allow to show help document to specified object or text.
*/
class IHelpViewer: virtual public IHelpInfoProvider
{
public:
	/**
		Show some help information for specified context.
	*/
	virtual void ShowHelp(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr) = 0;
};


} // namespace idoc


#endif // !idoc_IHelpViewer_included


