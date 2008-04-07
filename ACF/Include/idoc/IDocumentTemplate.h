#ifndef idoc_IDocumentTemplate_included
#define idoc_IDocumentTemplate_included


#include "idoc/idoc.h"


#include "istd/TIFactory.h"
#include "istd/CString.h"

#include "imod/IObserver.h"


namespace idoc
{		


class IDocument;


class IDocumentTemplate: virtual public istd::IPolymorphic
{
public:
	typedef istd::TIFactory<idoc::IDocument> IDocumentFactory;
	typedef istd::TIFactory<imod::IObserver> IViewFactory;


	/**
		Creates a document instance for document.
	*/
	virtual idoc::IDocument* CreateDocument() const = 0;

	/**
		Creates a view instance for document \c document of type \c viewTypeId.
	*/
	virtual imod::IObserver* AddView(idoc::IDocument& document, const std::string& viewTypeId = std::string()) const = 0;

	/**
	*/
	virtual istd::CStringList GetFileFilters() const = 0;

	/**
	*/
	virtual istd::CString GetDefaultDirectory() const = 0;

	/**
		Returns registered file extensions for this document template.
	*/
	virtual istd::CStringList GetFileExtensions() const = 0;

	/**
		Returns default title for an untitled document.
	*/
	virtual istd::CString GetDefaultTitle() const = 0;
};


} // namespace idoc


#endif // !idoc_IDocumentTemplate_included