#ifndef idoc_IMultiPageDocument_included
#define idoc_IMultiPageDocument_included


// Qt includes
#include <QtCore/QSizeF>
#include <QtCore/QString>

// ACF includes
#include <istd/IChangeable.h>


namespace iprm
{
	class IParamsSet;
}


namespace idoc
{


class IDocumentMetaInfo;


/**
	Simple interface for a structured document.
	
	A multi-page document is a container that manages a collection of pages,
	where each page owns its content. This interface is useful for documents
	that have a natural page-based structure, such as:
	- PDF documents
	- Word processing documents
	- Presentations
	- Multi-sheet spreadsheets
	- Multi-layer image documents
	
	Each page can have its own meta-information and content. The document itself
	also maintains document-level meta-information.
	
	\par Usage Example
	\code
	// Get or create a multi-page document
	idoc::IMultiPageDocument* mpDoc = GetMultiPageDocument();
	
	// Add a new page
	istd::IChangeable* page1 = mpDoc->InsertPage(nullptr, nullptr, -1);
	// Configure page1 content...
	
	// Add another page at the beginning
	istd::IChangeable* page0 = mpDoc->InsertPage(nullptr, nullptr, 0);
	
	// Iterate through all pages
	for (int i = 0; i < mpDoc->GetPagesCount(); ++i) {
		const istd::IChangeable& page = mpDoc->GetDocumentPage(i);
		const idoc::IDocumentMetaInfo* metaInfo = mpDoc->GetPageMetaInfo(i);
		if (metaInfo) {
			QString title = metaInfo->GetMetaInfo(
				idoc::IDocumentMetaInfo::MIT_TITLE).toString();
			qDebug() << "Page" << i << ":" << title;
		}
	}
	
	// Remove a specific page
	mpDoc->RemovePage(1);
	
	// Clear all pages
	mpDoc->ResetPages();
	
	// Access document-level metadata
	const idoc::IDocumentMetaInfo& docMeta = mpDoc->GetDocumentMetaInfo();
	QString docTitle = docMeta.GetMetaInfo(
		idoc::IDocumentMetaInfo::MIT_TITLE).toString();
	\endcode
	
	\sa IMultiPageDocumentProvider, IDocumentMetaInfo, CMultiPageDocumentFilePersistenceComp
	\ingroup DocumentBasedFramework
*/
class IMultiPageDocument: virtual public istd::IChangeable
{
public:
	/**
		Get number of pages in the document.
		\return	Total count of pages.
	*/
	virtual int GetPagesCount() const = 0;

	/**
		Get the document page.
		\param	pageIndex	Zero-based index of the page (0 to GetPagesCount()-1).
		\return	Reference to the page object containing the page content.
	*/
	virtual const istd::IChangeable& GetDocumentPage(int pageIndex) const = 0;

	/**
		Get meta info of the given page if exists.
		\param	pageIndex	Zero-based index of the page.
		\return	Pointer to page meta-information, or NULL if page has no meta-info.
	*/
	virtual const idoc::IDocumentMetaInfo* GetPageMetaInfo(int pageIndex) const = 0;

	/**
		Remove all pages and their content from the document.
		
		This operation clears the entire document, removing all pages.
	*/
	virtual void ResetPages() = 0;

	/**
		Insert a new document page at the given position.
		\param	pageMetaInfoPtr		Optional meta-information for the new page.
		\param	pageParameterPtr	Optional parameters for page creation.
		\param	position			Insertion position in the page container.
									If negative, the new page will be inserted after the last page.
		\return	Pointer to the object representing the contents of the new page.
		
		\par Example
		\code
		// Add page at the end
		istd::IChangeable* newPage = mpDoc->InsertPage(nullptr, nullptr, -1);
		
		// Insert page at the beginning
		istd::IChangeable* firstPage = mpDoc->InsertPage(nullptr, nullptr, 0);
		\endcode
	*/
	virtual istd::IChangeable* InsertPage(
				const idoc::IDocumentMetaInfo* pageMetaInfoPtr = NULL,
				const iprm::IParamsSet* pageParameterPtr = NULL,
				int position = -1) = 0;

	/**
		Remove the page at the given index.
		\param	pageIndex	Zero-based index of the page to remove.
		\return	True if the page was successfully removed, false otherwise.
	*/
	virtual bool RemovePage(int pageIndex) = 0;

	/**
		Get meta information about the document.
		\return	Reference to document-level meta-information.
		
		This is different from page meta-information - it describes the document as a whole.
	*/
	virtual const IDocumentMetaInfo& GetDocumentMetaInfo() const = 0;
};


typedef istd::TUniqueInterfacePtr<IMultiPageDocument> IMultiPageDocumentPtr;


} // namespace idoc


#endif // !idoc_IMultiPageDocument_included


