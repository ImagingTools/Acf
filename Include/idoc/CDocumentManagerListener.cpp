#include "idoc/CDocumentManagerListener.h"


#include "istd/TChangeNotifier.h"


namespace idoc
{		


// public methods

CDocumentManagerListener::CDocumentManagerListener()
	:m_currentDocumentModelPtr(NULL)
{
}


// reimplemented (imod::IModelSelection)

imod::IModel* CDocumentManagerListener::GetSelectedModel() const
{
	return m_currentDocumentModelPtr;
}


void CDocumentManagerListener::SetSelectedModel(imod::IModel* modelPtr)
{
	if (modelPtr != m_currentDocumentModelPtr){
		istd::CChangeNotifier changePtr(this);

		m_currentDocumentModelPtr = modelPtr;
	}
}


// reimplemented (imod::CSingleModelObserverBase)

void CDocumentManagerListener::OnUpdate(int updateFlags, istd::IPolymorphic* /* updateParamsPtr*/)
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

		imod::IModel* documentModelPtr = dynamic_cast<imod::IModel*>(documentPtr);

		SetSelectedModel(documentModelPtr);
	}
}


} // namespace idoc


