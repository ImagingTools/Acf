#ifndef idoc_IDocumentManager_included
#define idoc_IDocumentManager_included


#include <vector>

#include "istd/IChangeable.h"
#include "istd/CString.h"


namespace imod{
	class IModel;
	class IObserver;
}


namespace idoc
{		


class IDocument;


/**
\class IDocumentManager 
\brief ...
*/
class IDocumentManager: virtual public istd::IChangeable
{
public:
	enum ChangeFlags
	{
		DocumentCreated = 1,
		DocumentRemoved = 2,
		DocumentCountChanged = 4,
		DocumentActivationChanged = 8
	};


	/**
		Returns the active document. 
		If no document is active, the function return a \c NULL
	*/
	virtual idoc::IDocument* GetActiveDocument() const = 0;

	/**
		Returns the document with the index \c index. 
		If no document with this index exists, the function return a \c NULL
	*/
	virtual idoc::IDocument* GetDocument(int documentIndex) const = 0;

	/**
		Returns number of documents. 
	*/
	virtual int GetDocumentCount() const = 0;

	/**
		Creates a new document with the document ID \c documentId. 
	*/
	virtual idoc::IDocument* OnFileNew(const std::string& documentId) = 0;

	/**
		Saves active document. 
	*/
	virtual bool OnFileSave() = 0;

	/**
		Saves active document under some user defined name.
	*/
	virtual bool OnFileSaveAs() = 0;

	/**
		Opens document(s) from the file list. 
	*/
	virtual bool OnFileOpen(const std::string& documentId) = 0;

	/**
		Closes current view. 
	*/
	virtual bool OnFileClose() = 0;

	/**
		Returns supported document IDs
	*/
	virtual istd::CStringList GetDocumentIds() const = 0;
};


} // namespace idoc


#endif // !idoc_IDocumentManager_included