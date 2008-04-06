#ifndef idoc_CSingleDocumentTemplate_included
#define idoc_CSingleDocumentTemplate_included


#include "idoc/CDocumentTemplateBase.h"


namespace idoc
{	


class CSingleDocumentTemplate: public idoc::CDocumentTemplateBase
{
public:
	typedef idoc::CDocumentTemplateBase BaseClass;

	CSingleDocumentTemplate();
	CSingleDocumentTemplate(idoc::IDocumentManager* documentManagerPtr);
};


} // namespace idoc


#endif // !idoc_CSingleDocumentTemplate_included