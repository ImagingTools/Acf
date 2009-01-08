#include "idoc/CDocumentManagerBase.h"


// STL includes
#include <algorithm>

#include "istd/TChangeNotifier.h"

#include "imod/IModelEditor.h"

#include "iser/CArchiveTag.h"

#include "idoc/IDocumentTemplate.h"


namespace idoc
{		


CDocumentManagerBase::CDocumentManagerBase()
:	m_documentTemplatePtr(NULL), m_activeViewPtr(NULL)
{
}


void CDocumentManagerBase::SetDocumentTemplate(const IDocumentTemplate* documentTemplatePtr)
{
	m_documentTemplatePtr = documentTemplatePtr;
}


// reimplemented (idoc::IDocumentManager)

const idoc::IDocumentTemplate* CDocumentManagerBase::GetDocumentTemplate() const
{
	return m_documentTemplatePtr;
}


imod::IUndoManager* CDocumentManagerBase::GetUndoManagerForDocument(const istd::IChangeable* documentPtr) const
{
	int documentsCount = GetDocumentsCount();
	for (int i = 0; i < documentsCount; ++i){
		const DocumentInfo& info = GetDocumentInfo(i);
		
		if(info.documentPtr == documentPtr){
			return info.undoManagerPtr.GetPtr();
		}
	}

	return NULL;
}


int CDocumentManagerBase::GetDocumentsCount() const
{
	return m_documentInfos.GetCount();
}


istd::IChangeable& CDocumentManagerBase::GetDocumentFromIndex(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < m_documentInfos.GetCount());
	I_ASSERT(m_documentInfos.GetAt(index) != NULL);
	I_ASSERT(m_documentInfos.GetAt(index)->documentPtr.IsValid());

	return *(m_documentInfos.GetAt(index)->documentPtr);
}


istd::IPolymorphic* CDocumentManagerBase::GetActiveView() const
{
	return m_activeViewPtr;
}


void CDocumentManagerBase::SetActiveView(istd::IPolymorphic* viewPtr)
{
	if (m_activeViewPtr != viewPtr){
		istd::CChangeNotifier changePtr(this, ViewActivationChanged);

		m_activeViewPtr = viewPtr;
	}
}


istd::IChangeable* CDocumentManagerBase::GetDocumentFromView(const istd::IPolymorphic& view) const
{
	const DocumentInfo* infoPtr = GetDocumentInfoFromView(view);
	if (infoPtr != NULL){
		I_ASSERT(infoPtr != NULL);
		I_ASSERT(infoPtr->documentPtr.IsValid());

		return infoPtr->documentPtr.GetPtr();
	}

	return NULL;
}


std::string CDocumentManagerBase::GetDocumentTypeId(const istd::IChangeable& document) const
{
	int documentsCount = GetDocumentsCount();
	for (int i = 0; i < documentsCount; ++i){
		DocumentInfo& info = GetDocumentInfo(i);

		if (info.documentPtr == &document){
			return info.documentTypeId;
		}
	}

	return std::string();
}


istd::IChangeable* CDocumentManagerBase::FileNew(const std::string& documentTypeId, bool createView, const std::string& viewTypeId)
{
	istd::TDelPtr<DocumentInfo> newInfoPtr(CreateDocument(documentTypeId, createView, viewTypeId));
	if (newInfoPtr.IsValid() && RegisterDocument(newInfoPtr.GetPtr())){
		return newInfoPtr.PopPtr()->documentPtr.GetPtr();
	}

	return NULL;
}


bool CDocumentManagerBase::FileOpen(
			const std::string* documentTypeIdPtr,
			const istd::CString* fileNamePtr,
			bool createView,
			const std::string& viewTypeId,
			FileToTypeMap* loadedMapPtr)
{
	bool retVal = true;

	istd::CStringList files;

	if (fileNamePtr != NULL){
		files.push_back(*fileNamePtr);
	}
	else{
		files = GetOpenFileNames(documentTypeIdPtr);
	}

	for (		istd::CStringList::const_iterator iter = files.begin();
				iter != files.end();
				++iter){
		const istd::CString& fileName = *iter;

		std::string documentTypeId;
		if (OpenDocument(fileName, createView, viewTypeId, documentTypeId)){
			if (loadedMapPtr != NULL){
				loadedMapPtr->operator[](fileName) = documentTypeId;
			}
		}
		else{
			retVal = false;
		}
	}

	return retVal;
}


bool CDocumentManagerBase::FileSave(
			bool requestFileName,
			FileToTypeMap* savedMapPtr)
{
	if (m_documentTemplatePtr == NULL){
		return false;
	}

	DocumentInfo* infoPtr = GetActiveDocumentInfo();
	if (infoPtr == NULL){
		return false;
	}

	I_ASSERT(infoPtr->documentPtr.IsValid());

	istd::CString filePath = infoPtr->filePath;

	requestFileName  = requestFileName || filePath.IsEmpty();

	if (requestFileName){
		filePath = GetSaveFileName(infoPtr->documentTypeId);
		if (filePath.IsEmpty()){
			return true;
		}
	}

	for (		Views::const_iterator iter = infoPtr->views.begin();
				iter != infoPtr->views.end();
				++iter){
		const imod::IModelEditor* editorPtr = dynamic_cast<const imod::IModelEditor*>(iter->GetPtr());
		if ((editorPtr != NULL) && (!editorPtr->IsReadOnly())){
			editorPtr->UpdateModel();
		}
	}

	const iser::IFileLoader* loaderPtr = m_documentTemplatePtr->GetFileLoader(infoPtr->documentTypeId);
	if ((loaderPtr != NULL) && loaderPtr->SaveToFile(*infoPtr->documentPtr, filePath) == iser::IFileLoader::StateOk){
		if ((infoPtr->filePath != filePath) || infoPtr->isDirty){
			istd::CChangeNotifier notifierPtr(this);

			infoPtr->filePath = filePath;
			infoPtr->isDirty = false;
		}

		if (savedMapPtr != NULL){
			savedMapPtr->operator[](filePath) = infoPtr->documentTypeId;
		}

		return true;
	}

	return false;
}


bool CDocumentManagerBase::FileClose()
{
	int documentsCount = GetDocumentsCount();
	for (int i = 0; i < documentsCount; ++i){
		DocumentInfo& info = GetDocumentInfo(i);

		Views::iterator findIter = std::find(info.views.begin(), info.views.end(), m_activeViewPtr);
		if (findIter != info.views.end()){
			if (info.isDirty){
				if (!QueryDocumentClose(info)){
					return false;
				}
			}

			I_ASSERT(findIter->IsValid());
			OnViewRemoved(findIter->GetPtr());

			info.views.erase(findIter);	// remove active view

			m_activeViewPtr = NULL;

			if (info.views.empty()){	// last view was removed
				int changeFlags = DocumentRemoved | DocumentCountChanged;
				// if last document was closed, force view activation update:
				if (m_documentInfos.GetCount() == 1){
					changeFlags |= ViewActivationChanged;
				}

				istd::CChangeNotifier notifier(this, changeFlags);

				m_documentInfos.RemoveAt(i);
			}

			return true;
		}
	}

	return false;
}


// protected methods

istd::IChangeable* CDocumentManagerBase::OpenDocument(
			const istd::CString& filePath,
			bool createView,
			const std::string& viewTypeId,
			std::string& documentTypeId)
{
	if (filePath.IsEmpty() || (m_documentTemplatePtr == NULL)){
		return NULL;
	}

	DocumentInfo* existingInfoPtr = GetDocumentInfoFromPath(filePath);
	if (existingInfoPtr != NULL){
		I_ASSERT(existingInfoPtr->documentPtr.IsValid());

		if (createView){
			istd::IPolymorphic* viewPtr = m_documentTemplatePtr->CreateView(
						existingInfoPtr->documentTypeId,
						existingInfoPtr->documentPtr.GetPtr(),
						viewTypeId);
			if (viewPtr != NULL){
				existingInfoPtr->views.push_back(ViewPtr());
				existingInfoPtr->views.back().SetPtr(viewPtr);

				OnViewRegistered(viewPtr);
			}
		}

		documentTypeId = existingInfoPtr->documentTypeId;

		return existingInfoPtr->documentPtr.GetPtr();
	}

	IDocumentTemplate::Ids documentIds = m_documentTemplatePtr->GetDocumentTypeIdsForFile(filePath);

	if (!documentIds.empty()){
		documentTypeId = documentIds.front();
		istd::TDelPtr<DocumentInfo> infoPtr(CreateDocument(documentTypeId, createView, viewTypeId));
		if (infoPtr.IsValid()){
			I_ASSERT(infoPtr->documentPtr.IsValid());

			infoPtr->filePath = filePath;
			infoPtr->documentTypeId = documentTypeId;
			iser::IFileLoader* loaderPtr = m_documentTemplatePtr->GetFileLoader(documentTypeId);
			if (		(loaderPtr != NULL) &&
						(loaderPtr->LoadFromFile(*infoPtr->documentPtr, filePath) == iser::IFileLoader::StateOk)){
				RegisterDocument(infoPtr.GetPtr());

				return infoPtr.PopPtr()->documentPtr.GetPtr();
			}
		}
	}

	return NULL;
}


void CDocumentManagerBase::CloseAllDocuments()
{
	istd::CChangeNotifier notifierPtr(this, DocumentCountChanged | DocumentRemoved);

	m_documentInfos.Reset();
}


CDocumentManagerBase::DocumentInfo& CDocumentManagerBase::GetDocumentInfo(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < GetDocumentsCount());

	DocumentInfo* retVal = const_cast<DocumentInfo*>(m_documentInfos.GetAt(index));
	I_ASSERT(retVal != NULL);

	return *retVal;
}


CDocumentManagerBase::DocumentInfo* CDocumentManagerBase::GetActiveDocumentInfo() const
{
	const istd::IPolymorphic* viewPtr = GetActiveView();
	if (viewPtr != NULL){
		return GetDocumentInfoFromView(*viewPtr);
	}

	return NULL;
}


CDocumentManagerBase::DocumentInfo* CDocumentManagerBase::GetDocumentInfoFromView(const istd::IPolymorphic& view) const
{
	int documentsCount = GetDocumentsCount();
	for (int i = 0; i < documentsCount; ++i){
		DocumentInfo& info = GetDocumentInfo(i);

		Views::iterator findIter = std::find(info.views.begin(), info.views.end(), &view);
		if (findIter != info.views.end()){
			return &info;
		}
	}

	return NULL;
}


CDocumentManagerBase::DocumentInfo* CDocumentManagerBase::GetDocumentInfoFromPath(const istd::CString& filePath) const
{
	int documentsCount = GetDocumentsCount();
	for (int i = 0; i < documentsCount; ++i){
		DocumentInfo& info = GetDocumentInfo(i);

		if (info.filePath == filePath){
			return &info;
		}
	}

	return NULL;
}


CDocumentManagerBase::DocumentInfo* CDocumentManagerBase::CreateDocument(const std::string& documentTypeId, bool createView, const std::string& viewTypeId) const
{
	if (m_documentTemplatePtr != NULL){
		istd::IChangeable* documentPtr = m_documentTemplatePtr->CreateDocument(documentTypeId);

		istd::TDelPtr<DocumentInfo> infoPtr(new DocumentInfo(
					const_cast<CDocumentManagerBase*>(this),
					documentTypeId,
					documentPtr,
					m_documentTemplatePtr->CreateUndoManager(documentTypeId, documentPtr)));

		if (infoPtr->documentPtr.IsValid()){
			imod::IModel* documentModelPtr = dynamic_cast<imod::IModel*>(documentPtr);
			if (documentModelPtr != NULL){
				documentModelPtr->AttachObserver(infoPtr.GetPtr());
			}

			if (createView){
				istd::IPolymorphic* viewPtr = m_documentTemplatePtr->CreateView(
							documentTypeId,
							infoPtr->documentPtr.GetPtr(),
							viewTypeId);
				if (viewPtr == NULL){
					return NULL;
				}

				infoPtr->views.push_back(ViewPtr());
				infoPtr->views.back().SetPtr(viewPtr);
			}

			return infoPtr.PopPtr();
		}
	}

	return NULL;
}


bool CDocumentManagerBase::RegisterDocument(DocumentInfo* infoPtr)
{
	I_ASSERT(infoPtr != NULL);

	istd::CChangeNotifier changePtr(this, DocumentCountChanged | DocumentCreated);

	infoPtr->isDirty = false;

	m_documentInfos.PushBack(infoPtr);

	for (		Views::iterator iter = infoPtr->views.begin();
				iter != infoPtr->views.end();
				++iter){
		I_ASSERT(iter->IsValid());

		OnViewRegistered(iter->GetPtr());
	}

	return true;
}


void CDocumentManagerBase::OnViewRegistered(istd::IPolymorphic* /*viewPtr*/)
{
}


void CDocumentManagerBase::OnViewRemoved(istd::IPolymorphic* /*viewPtr*/)
{
}


// protected methods of embedded class DocumentInfo

// reimplemented (imod::CSingleModelObserverBase)

void CDocumentManagerBase::DocumentInfo::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if (!isDirty){
		istd::CChangeNotifier notifier(parentPtr);

		isDirty = true;
	}
}


} // namespace idoc


