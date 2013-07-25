#ifndef idoc_IMultiPageDocument_included
#define idoc_IMultiPageDocument_included


// Qt includes
#include <QtCore/QSizeF>
#include <QtCore/QString>

// ACF includes
#include <istd/IChangeable.h>


namespace idoc
{


class IDocumentMetaInfo;


/**
	Simple interface for a structured document.
	Document is container of the pages. A page is the owner of the content.
*/
class IMultiPageDocument: virtual public istd::IChangeable
{
public:
	/**
		Get number of pages in the document.
	*/
	virtual int GetPagesCount() const = 0;

	/**
		Get the document page.
	*/
	virtual const istd::IChangeable& GetDocumentPage(int pageIndex) const = 0;

	/**
		Remove all pages and their content from the document.
	*/
	virtual void ResetPages() = 0;

	/**
		Insert a new document page at the given position.
		\param pageTitle - Title of the page.
		\param pageSize - Physical dimension of the page in mm.
		\param Insertion position in the page container. If this value is negative, the new page will be inserted after the last page of the document.
	*/
	virtual istd::IChangeable* InsertPage(
				const QString& pageTitle = QString(),
				const QSizeF& pageSize = QSizeF(),
				int position = -1) = 0;

	/**
		Remove the page at the given index.
	*/
	virtual bool RemovePage(int pageIndex) = 0;

	/**
		Get meta information about the document. 
	*/
	virtual const IDocumentMetaInfo& GetDocumentMetaInfo() const = 0;
};


} // namespace idoc


#endif // !idoc_IMultiPageDocument_included


