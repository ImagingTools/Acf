#ifndef idoc_CMultiDocumentTemplate_included
#define idoc_CMultiDocumentTemplate_included


#include "idoc/CDocumentTemplateBase.h"


namespace idoc
{		


class CMultiDocumentTemplate: public idoc::CDocumentTemplateBase
{
public:
	typedef idoc::CDocumentTemplateBase BaseClass;

	CMultiDocumentTemplate();
	CMultiDocumentTemplate(idoc::IDocumentManager* documentManagerPtr, int maxDocumentCount = std::numeric_limits<int>::infinity());
};


} // namespace idoc


#endif // !idoc_CMultiDocumentTemplate_included