#include "idoc/CSingleDocumentTemplateComp.h"


namespace idoc
{		


// reimplemented (idoc::IDocumentTemplate)

iser::IFileLoader* CSingleDocumentTemplateComp::GetFileLoader(const std::string& documentTypeId) const
{
	if (IsDocumentTypeSupported(documentTypeId)){
		return m_fileLoaderCompPtr.GetPtr();
	}
	else{
		return NULL;
	}
}


istd::IChangeable* CSingleDocumentTemplateComp::CreateDocument(const std::string& documentTypeId) const
{
	if (m_documentCompFact.IsValid() && IsDocumentTypeSupported(documentTypeId)){
		return m_documentCompFact.CreateInstance();
	}

	return NULL;
}


istd::IPolymorphic* CSingleDocumentTemplateComp::CreateView(
			const std::string& documentTypeId,
			istd::IChangeable* documentPtr,
			const std::string& viewTypeId) const
{
	I_ASSERT(documentPtr != NULL);

	imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(documentPtr);

	if (		(modelPtr != NULL) &&
				m_viewCompFact.IsValid() &&
				IsDocumentTypeSupported(documentTypeId) &&
				IsViewTypeSupported(viewTypeId)){
		istd::TDelPtr<imod::IObserver> viewPtr(m_viewCompFact.CreateInstance());
		if (viewPtr.IsValid() && modelPtr->AttachObserver(viewPtr.GetPtr())){
			return viewPtr.PopPtr();
		}
	}

	return NULL;
}


// reimplemented (icomp::CComponentBase)

void CSingleDocumentTemplateComp::OnComponentCreated()
{
	I_ASSERT(m_documentTypeIdAttrPtr.IsValid());
	SetDocumentTypeId((*m_documentTypeIdAttrPtr).ToString());

	I_ASSERT(m_defaultDirectoryAttrPtr.IsValid());
	SetDefaultDirectory(*m_defaultDirectoryAttrPtr);

	int featureFlags = 0;
	if (m_isNewSupportedAttrPtr.IsValid() && m_isNewSupportedAttrPtr->GetValue()){
		featureFlags |= idoc::IDocumentTemplate::New;
	}

	if (m_isEditSupportedAttrPtr.IsValid() && m_isEditSupportedAttrPtr->GetValue()){
		featureFlags |= idoc::IDocumentTemplate::Edit;
	}

	SetSupportedFeatures(featureFlags);
}


} // namespace idoc

