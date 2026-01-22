// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iqt2d/CSceneBinderComp.h>


// ACF includes
#include <iview/IShapeView.h>


namespace iqt2d
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CSceneBinderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_sceneProviderCompPtr.IsValid() && m_extenderCompPtr.IsValid()){
		m_extenderCompPtr->AddItemsToScene(m_sceneProviderCompPtr.GetPtr(), IViewExtender::SF_DIRECT);

		iview::IShapeView* viewPtr = m_sceneProviderCompPtr->GetView();
		if (viewPtr != NULL){
			viewPtr->Update();
		}
	}
}


void CSceneBinderComp::OnComponentDestroyed()
{
	if (m_sceneProviderCompPtr.IsValid() && m_extenderCompPtr.IsValid()){
		m_extenderCompPtr->RemoveItemsFromScene(m_sceneProviderCompPtr.GetPtr());

		iview::IShapeView* viewPtr = m_sceneProviderCompPtr->GetView();
		if (viewPtr != NULL){
			viewPtr->Update();
		}
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace iqt2d


