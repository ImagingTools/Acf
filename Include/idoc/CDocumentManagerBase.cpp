#include "idoc/CDocumentManagerBase.h"


#include "idoc/IDocumentTemplate.h"


namespace idoc
{		


CDocumentManagerBase::CDocumentManagerBase()
:	m_documentTemplatePtr(NULL)
{
}


// reimplemented (idoc::IDocumentManager)

int CDocumentManagerBase::GetAllowedOperationFlags(const istd::IPolymorphic* viewPtr) const
{
	int retVal = m_baseAllowedFlags;

	if (viewPtr == NULL){
		viewPtr = GetActiveView();
		if (viewPtr == NULL){
			return retVal;
		}
	}

	DocumentInfo docInfo;
	const istd::IChangeable* documentPtr = GetDocumentFromView(*viewPtr, &docInfo);
	if (documentPtr == NULL){
		return retVal;
	}

	if ((m_documentTemplatePtr != NULL) && (documentPtr != NULL)){
		std::string docTypeId = GetDocumentTypeId(*documentPtr);
		iser::IFileLoader* loaderPtr = m_documentTemplatePtr->GetFileLoader(docTypeId);
		if (loaderPtr != NULL){
			if (docInfo.filePath.IsEmpty() || loaderPtr->IsOperationSupported(
						documentPtr,
						&docInfo.filePath,
						iser::IFileLoader::QF_SAVE | iser::IFileLoader::QF_FILE)){
				retVal |= OF_FILE_SAVE;
			}

			if (loaderPtr->IsOperationSupported(
						documentPtr,
						NULL,
						iser::IFileLoader::QF_SAVE | iser::IFileLoader::QF_FILE)){
				retVal |= OF_FILE_SAVE_AS;
			}
		}
	}

	if (!docInfo.filePath.IsEmpty()){
		retVal |= OF_KNOWN_PATH;
	}

	return retVal;
}


// reimplemented (idoc::IDocumentTypesInfo)

bool CDocumentManagerBase::IsFeatureSupported(int featureFlags, const std::string& documentTypeId) const
{
	if (m_documentTemplatePtr != NULL){
		return m_documentTemplatePtr->IsFeatureSupported(featureFlags, documentTypeId);
	}

	return false;
}


IDocumentTypesInfo::Ids CDocumentManagerBase::GetDocumentTypeIds() const
{
	if (m_documentTemplatePtr != NULL){
		return m_documentTemplatePtr->GetDocumentTypeIds();
	}

	return Ids();
}


istd::CString CDocumentManagerBase::GetDocumentTypeName(const std::string& documentTypeId) const
{
	if (m_documentTemplatePtr != NULL){
		return m_documentTemplatePtr->GetDocumentTypeName(documentTypeId);
	}

	return "";
}


iser::IFileTypeInfo* CDocumentManagerBase::GetDocumentFileTypeInfo(const std::string& documentTypeId) const
{
	if (m_documentTemplatePtr != NULL){
		return m_documentTemplatePtr->GetFileLoader(documentTypeId);
	}

	return NULL;
}


IDocumentTypesInfo::Ids CDocumentManagerBase::GetDocumentTypeIdsForFile(const istd::CString& filePath) const
{
	if (m_documentTemplatePtr != NULL){
		return m_documentTemplatePtr->GetDocumentTypeIdsForFile(filePath);
	}

	return Ids();
}


istd::CString CDocumentManagerBase::GetDefaultDirectory(const istd::CString& sugestedDir, const std::string* documentTypeIdPtr) const
{
	if (m_documentTemplatePtr != NULL){
		return m_documentTemplatePtr->GetDefaultDirectory(sugestedDir, documentTypeIdPtr);
	}

	return "";
}


// protected methods

const idoc::IDocumentTemplate* CDocumentManagerBase::GetDocumentTemplate() const
{
	return m_documentTemplatePtr;
}


void CDocumentManagerBase::SetDocumentTemplate(const IDocumentTemplate* documentTemplatePtr)
{
	m_documentTemplatePtr = documentTemplatePtr;

	m_baseAllowedFlags = 0;

	if (m_documentTemplatePtr != NULL){
		m_baseAllowedFlags |= OF_FILE_NEW;

		IDocumentTemplate::Ids docTypeIds = m_documentTemplatePtr->GetDocumentTypeIds();
		for (		IDocumentTemplate::Ids::const_iterator iter = docTypeIds.begin();
					iter != docTypeIds.end();
					++iter){
			const std::string& docTypeId = *iter;

			iser::IFileLoader* loaderPtr = m_documentTemplatePtr->GetFileLoader(docTypeId);
			if (		(loaderPtr != NULL) &&
						loaderPtr->IsOperationSupported(NULL, NULL, iser::IFileLoader::QF_LOAD | iser::IFileLoader::QF_FILE)){
				m_baseAllowedFlags |= OF_FILE_OPEN;

				break;
			}
		}
	}
}


} // namespace idoc


