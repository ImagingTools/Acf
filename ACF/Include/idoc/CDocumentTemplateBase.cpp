#include "idoc/CDocumentTemplateBase.h"

#include <algorithm>


#include "idoc/IDocument.h"
#include "idoc/IDocumentManager.h"


namespace idoc
{		

CDocumentTemplateBase::CDocumentTemplateBase(idoc::IDocumentManager* documentManagerPtr, int maxDocumentCount)
	:m_documentManagerPtr(documentManagerPtr),
	m_maxDocumentCount(maxDocumentCount),
	m_documentFactoryPtr(NULL),
	m_viewFactoryPtr(NULL)
{
}


void CDocumentTemplateBase::RegisterDocumentFactory(IDocumentFactory* documentFactoryPtr)
{
	m_documentFactoryPtr = documentFactoryPtr;
}


void CDocumentTemplateBase::RegisterViewFactory(IViewFactory* viewFactoryPtr)
{	
	m_viewFactoryPtr = viewFactoryPtr;
}


void CDocumentTemplateBase::SetFileFilters(const istd::CStringList& fileFilters)
{
	m_fileFilters = fileFilters;
}


void CDocumentTemplateBase::SetDefaultDirectory(const istd::CString& defaultDirectory)
{
	m_defaultDirectory = defaultDirectory;
}


void CDocumentTemplateBase::SetFileExtensions(const istd::CStringList& fileExtensions)
{	
	m_fileExtensions = fileExtensions;
}


void CDocumentTemplateBase::SetDefaultTitle(const istd::CString& defaultTitle)
{
	m_defaultTitle = defaultTitle;
}

	
// reimplemented (idoc::IDocumentTemplate)

idoc::IDocument* CDocumentTemplateBase::CreateDocument() const
{
	if (m_documentFactoryPtr == NULL){
		return NULL;
	}

	idoc::IDocument* documentPtr = m_documentFactoryPtr->CreateInstance();
	if (documentPtr == NULL){
		return NULL;
	}

	// add default view:
	imod::IObserver* viewPtr = AddView(*documentPtr);
	if (viewPtr == NULL){
		delete documentPtr;

		return NULL;
	}

	return documentPtr;
}


imod::IObserver* CDocumentTemplateBase::AddView(idoc::IDocument& document, const std::string& viewTypeId) const
{
	imod::IObserver* viewPtr = CreateView(viewTypeId);
	if (viewPtr != NULL){
		bool retVal = document.AddView(viewPtr);
		if (!retVal){
			delete viewPtr;

			return NULL;
		}
	}

	return viewPtr;
}


istd::CStringList CDocumentTemplateBase::GetFileFilters() const
{
	return m_fileFilters;
}


istd::CString CDocumentTemplateBase::GetDefaultDirectory() const
{
	return m_defaultDirectory;
}


istd::CStringList CDocumentTemplateBase::GetFileExtensions() const
{
	return m_fileExtensions;
}


istd::CString CDocumentTemplateBase::GetDefaultTitle() const
{
	return m_defaultTitle;
}


// protected methods

imod::IObserver* CDocumentTemplateBase::CreateView(const std::string& viewTypeId) const
{
	if (m_viewFactoryPtr == NULL){
		return NULL;
	}
	
	return m_viewFactoryPtr->CreateInstance(viewTypeId);
}


} // namespace idoc


