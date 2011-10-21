#ifndef idoc_IHelpFileProvider_included
#define idoc_IHelpFileProvider_included


// ACF includes
#include "istd/IPolymorphic.h"
#include "istd/CString.h"

#include "idoc/IHelpInfoProvider.h"


namespace idoc
{


/**
	Allow to show help document to specified object or text.
*/
class IHelpFileProvider: virtual public IHelpInfoProvider
{
public:
	/**
		Get access to help file path for specified help information.
	*/
	virtual istd::CString GetHelpFilePath(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr) const = 0;
};


} // namespace idoc


#endif // !idoc_IHelpFileProvider_included


