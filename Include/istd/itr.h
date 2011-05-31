#ifndef istd_itr_included
#define istd_itr_included


#include "istd/ILocalizer.h"
#include "istd/CString.h"
#include "istd/CStaticServicesProvider.h"


/**
	Macro for defining string localization function in Non-Qt context.
*/
#define I_DECLARE_TR_FUNCTION(ContextName)\
	static istd::CString tr(const istd::CString& text, const std::string& disambiguation = std::string())\
	{\
		istd::ILocalizer* translatorPtr = istd::GetService<istd::ILocalizer>();\
		if (translatorPtr != NULL){\
			translatorPtr->GetTranslatedText(#ContextName, text, disambiguation);\
		}\
		return text;\
	}

#endif // !istd_itr_included


