#ifndef idoc_CDocumentManagerBase_included
#define idoc_CDocumentManagerBase_included


#include <map>


#include "idoc/IDocumentManager.h"
#include "idoc/IDocument.h"

#include "istd/TIFactory.h"
#include "istd/TPointerVector.h"
#include "istd/TDelPtr.h"

#include "imod/TMultiModelObserverBase.h"


namespace idoc
{		


class CDocumentManagerBase: public idoc::IDocumentManager,
							public imod::TMultiModelObserverBase<idoc::IDocument>
{
public:
	typedef imod::TMultiModelObserverBase<idoc::IDocument> BaseClass;

	CDocumentManagerBase();
	virtual ~CDocumentManagerBase();

	virtual idoc::IDocument* OpenDocument(const istd::CString& fileName);
	void RegisterDocumentTemplate(const std::string& documentId, idoc::IDocumentTemplate* documentTemplate);

	// reimplemented (idoc::IDocumentManager)
	virtual idoc::IDocument* GetActiveDocument() const;
	virtual imod::IObserver* GetActiveView() const;
	virtual idoc::IDocument* GetDocument(int documentIndex) const;
	virtual int GetDocumentCount() const;
	virtual idoc::IDocument* OnFileNew(const std::string& documentId);
	imod::IObserver* OnWindowNew(const std::string& viewTypeId);
	virtual bool OnFileSave();
	virtual bool OnFileSaveAs();
	virtual bool OnFileOpen(const std::string& documentId);
	virtual bool OnFileClose();
	virtual istd::CStringList GetDocumentIds() const;

protected:
	std::string GetTemplateIdFromFile(const istd::CString& fileName) const;
	bool CloseDocument(int index);
	void CloseAllDocuments();
	void RemoveDocument(IDocument* document);
	istd::CString CalculateCopyName(const istd::CString& documentTitle) const;

	// abstract methods
	
	/**
		Gets save file name.
	*/
	virtual istd::CString GetSaveFileName(const idoc::IDocumentTemplate& documentTemplate) const = 0;

	/**
		Gets open file names.
	*/
	virtual istd::CStringList GetOpenFileNames(const idoc::IDocumentTemplate& documentTemplate) const  = 0;

private:
	idoc::IDocumentTemplate* GetTemplateFromId(const std::string& templateId) const;

protected:
	typedef istd::TPointerVector<idoc::IDocument> Documents;
	typedef istd::TIFactory<imod::IObserver> IViewFactory;
	typedef std::map<std::string, IDocumentTemplate*> DocumentTemplateMap;

	DocumentTemplateMap m_documentTemplateMap;
	Documents m_documents;
	int m_activeIndex;
	int m_untitledIndex;
};


} // namespace idoc


#endif // idoc_CDocumentManagerBase_included