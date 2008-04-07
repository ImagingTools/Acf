#ifndef idoc_CDocument_included
#define idoc_CDocument_included


#include "idoc/CDocumentBase.h"

#include "imod/CModelChangeObserver.h"


namespace idoc
{		


class IDocumentTemplate;


class CDocument: public idoc::CDocumentBase, public imod::CModelChangeObserver
{
public:
	typedef idoc::CDocumentBase BaseClass;

	CDocument(idoc::IDocumentTemplate* documentTemplatePtr = NULL);

	// reimplemented (idoc::CDocumentBase)
	virtual void SetContent(imod::IModel* modelPtr);

	// reimplemented (idoc::IDocument)
	virtual bool IsModified() const;
};


} // namespace idoc


#endif // !idoc_CDocument_included

