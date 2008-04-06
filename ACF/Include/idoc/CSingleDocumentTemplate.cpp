#include "idoc/CSingleDocumentTemplate.h"


namespace idoc
{		


CSingleDocumentTemplate::CSingleDocumentTemplate()
	:BaseClass(NULL, 0)
{
}


CSingleDocumentTemplate::CSingleDocumentTemplate(idoc::IDocumentManager* documentManagerPtr)
	:BaseClass(documentManagerPtr, 1)
{
}


} // namespace idoc