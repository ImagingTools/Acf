#include "idoc/CDocumentManagerListenerComp.h"


#include "istd/TChangeNotifier.h"


namespace idoc
{		


// public methods

CDocumentManagerListenerComp::CDocumentManagerListenerComp()
	:m_currentDocumentModelPtr(NULL)
{
}


// reimplemented (imod::IModelSelection)

imod::IModel* CDocumentManagerListenerComp::GetSelectedModel() const
{
	return m_currentDocumentModelPtr;
}


void CDocumentManagerListenerComp::SetSelectedModel(imod::IModel* modelPtr)
{
	if (modelPtr != m_currentDocumentModelPtr){
		istd::CChangeNotifier changePtr(this);

		m_currentDocumentModelPtr = modelPtr;
	}
}


// reimplemented (imod::CSingleModelObserverBase)

void CDocumentManagerListenerComp::OnUpdate(int updateFlags, istd::IPolymorphic* /* updateParamsPtr*/)
{
	idoc::IDocumentManager* documentManagerPtr = GetObjectPtr();
	if (documentManagerPtr == NULL){
		m_currentDocumentModelPtr = NULL;
		return;
	}

	bool updateModel = 
				(updateFlags & IDocumentManager::DocumentRemoved) != 0 || 
				(updateFlags & IDocumentManager::ViewActivationChanged) != 0; 

	if (updateModel){
		istd::IChangeable* documentPtr = NULL;
		istd::IPolymorphic* activeViewPtr = documentManagerPtr->GetActiveView();

		if (activeViewPtr != NULL){
			documentPtr = documentManagerPtr->GetDocumentFromView(*activeViewPtr);
		}

		istd::IPolymorphic* modelSourcePtr = documentPtr;
		if (m_useModelFromViewAttrPtr.IsValid()  && *m_useModelFromViewAttrPtr){
			modelSourcePtr = activeViewPtr;
		}

		imod::IModel* newModelPtr = dynamic_cast<imod::IModel*>(modelSourcePtr);

		SetSelectedModel(newModelPtr);
	}
}


} // namespace idoc


