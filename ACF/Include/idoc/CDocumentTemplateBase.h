#ifndef idoc_CDocumentTemplateBase_included
#define idoc_CDocumentTemplateBase_included

#include <limits>


#include "idoc/IDocumentTemplate.h"


namespace idoc
{		


class IDocument;
class IDocumentManager;


class CDocumentTemplateBase: virtual public idoc::IDocumentTemplate
{
public:
	CDocumentTemplateBase(idoc::IDocumentManager* documentManagerPtr, int maxDocumentCount = std::numeric_limits<int>::infinity());

	virtual void RegisterDocumentFactory(IDocumentFactory* documentFactoryPtr);
	virtual void RegisterViewFactory(IViewFactory* documentFactoryPtr);

	virtual void SetFileFilters(const istd::CStringList& fileFilters);
	virtual void SetDefaultDirectory(const istd::CString& defaultDirectory);
	virtual void SetFileExtensions(const istd::CStringList& fileExtensions);
	virtual void SetDefaultTitle(const istd::CString& defaultTitle);

	// reimplemented (idoc::IDocumentTemplate)
	virtual idoc::IDocument* CreateDocument(const std::string& documentTypeId) const;
	virtual imod::IObserver* CreateView(const idoc::IDocument& document, const std::string& viewTypeId = std::string()) const;
	virtual istd::CStringList GetFileFilters() const;
	virtual istd::CString GetDefaultDirectory() const;
	virtual istd::CStringList GetFileExtensions() const;
	virtual istd::CString GetDefaultTitle() const;

protected:
	idoc::IDocumentManager* m_documentManagerPtr;
	int m_maxDocumentCount;

private:
	istd::CStringList m_fileFilters;
	istd::CString m_defaultDirectory;
	istd::CString m_defaultTitle;
	istd::CStringList m_fileExtensions;

	IDocumentFactory* m_documentFactoryPtr;
	IViewFactory* m_viewFactoryPtr;
};


} // namespace idoc


#endif // !idoc_CDocumentTemplateBase_included

