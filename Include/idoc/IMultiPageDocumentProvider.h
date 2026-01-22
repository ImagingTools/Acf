// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// ACF includes
#include <istd/IChangeable.h>


namespace idoc
{


class IMultiPageDocument;


/**
	Common interface for a provider of a multi-page document.
	
	This interface provides access to a multi-page document instance.
	It's typically implemented by components that manage or contain multi-page documents.
	
	The provider pattern allows separation between document ownership and document access,
	enabling flexible architectures where documents can be managed by different components.
	
	\par Usage Example
	\code
	// Get provider from a component
	idoc::IMultiPageDocumentProvider* provider = GetDocumentProvider();
	
	// Access the document
	const idoc::IMultiPageDocument* doc = provider->GetDocument();
	if (doc) {
		qDebug() << "Document has" << doc->GetPagesCount() << "pages";
		
		// Work with the document
		for (int i = 0; i < doc->GetPagesCount(); ++i) {
			const istd::IChangeable& page = doc->GetDocumentPage(i);
			// Process page...
		}
	}
	\endcode
	
	\sa IMultiPageDocument
	\ingroup DocumentBasedFramework
*/
class IMultiPageDocumentProvider: virtual public istd::IChangeable
{
public:
	/**
		Get multi-page document instance.
		\return	Pointer to the multi-page document, or NULL if no document is available.
	*/
	virtual const IMultiPageDocument* GetDocument() const = 0;
};


} // namespace idoc




