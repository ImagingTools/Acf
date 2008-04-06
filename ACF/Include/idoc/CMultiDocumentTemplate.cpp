#include "idoc/CMultiDocumentTemplate.h"


namespace idoc
{		


CMultiDocumentTemplate::CMultiDocumentTemplate()
:BaseClass(NULL, 0)
{
}


CMultiDocumentTemplate::CMultiDocumentTemplate(idoc::IDocumentManager* documentManagerPtr, int maxDocumentCount)
	:BaseClass(documentManagerPtr, maxDocumentCount)
{
}


} // namespace idoc
