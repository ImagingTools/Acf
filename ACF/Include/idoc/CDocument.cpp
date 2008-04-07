#include "idoc/CDocument.h"


namespace idoc
{		


// public  methods

CDocument::CDocument(idoc::IDocumentTemplate* documentTemplatePtr)
	:BaseClass(documentTemplatePtr)
{
}


// reimplemented (idoc::CDocumentBase)

void CDocument::SetContent(imod::IModel* modelPtr)
{
	BaseClass::SetContent(modelPtr);

	SetModelPtr(modelPtr);
}


// reimplemented (idoc::IDocument)

bool CDocument::IsModified() const
{
	return IsDirty();
}


} // namespace idoc

