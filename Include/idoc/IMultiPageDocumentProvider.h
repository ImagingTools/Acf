#ifndef idoc_IMultiPageDocumentProvider_included
#define idoc_IMultiPageDocumentProvider_included


// ACF includes
#include "istd/IChangeable.h"


namespace idoc
{


class IMultiPageDocument;


class IMultiPageDocumentProvider: virtual public istd::IChangeable
{
public:
	virtual const IMultiPageDocument* GetDocument() const = 0;
};


} // namespace idoc


#endif // !idoc_IMultiPageDocumentProvider_included


