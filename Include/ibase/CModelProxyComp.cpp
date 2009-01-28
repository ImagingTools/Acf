#include "ibase/CModelProxyComp.h"


namespace ibase
{


CModelProxyComp::CModelProxyComp()
	:m_modelSelectionListener(*this)
{
}


// reimplemented (icomp::IComponent)

void CModelProxyComp::OnComponentCreated()
{
	if (m_modelSelectionCompPtr.IsValid() && m_selectionModelCompPtr.IsValid()){
		m_selectionModelCompPtr->AttachObserver(&m_modelSelectionListener);
	}
}


void CModelProxyComp::OnComponentDestroyed()
{
	if (m_modelSelectionCompPtr.IsValid() && m_selectionModelCompPtr.IsValid()){
		m_selectionModelCompPtr->DetachObserver(&m_modelSelectionListener);
	}
}


// public methods of embedded class ModelSelectionListener

CModelProxyComp::ModelSelectionListener::ModelSelectionListener(CModelProxyComp& parent)
	:m_parent(parent)
{
}


// reimplemented (imod::CSingleModelObserverBase)

void CModelProxyComp::ModelSelectionListener::AfterUpdate(imod::IModel* /*modelPtr*/, int/* updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	imod::IModelSelection* selectionPtr = GetObjectPtr();
	if (selectionPtr != NULL){
		m_parent.SetModelPtr(selectionPtr->GetSelectedModel());
	}
}


} // namespace ibase


