#include "iqt2d/CSceneBinderComp.h"


namespace iqt2d
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CSceneBinderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_sceneProviderCompPtr.IsValid() && m_extenderCompPtr.IsValid()){
		m_extenderCompPtr->AddItemsToScene(m_sceneProviderCompPtr.GetPtr(), ISceneExtender::SF_DIRECT);
	}
}


void CSceneBinderComp::OnComponentDestroyed()
{
	if (m_sceneProviderCompPtr.IsValid() && m_extenderCompPtr.IsValid()){
		m_extenderCompPtr->RemoveItemsFromScene(m_sceneProviderCompPtr.GetPtr());
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace iqt2d


