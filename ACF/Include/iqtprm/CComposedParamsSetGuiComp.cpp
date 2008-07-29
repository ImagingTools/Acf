#include "iqtprm/CComposedParamsSetGuiComp.h"


// Qt includes
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QToolBox>

#include "imod/IModel.h"
#include "imod/IObserver.h"


namespace iqtprm
{


// reimplemented (imod::IModelEditor)

void CComposedParamsSetGuiComp::UpdateModel() const
{
	int editorsCount = m_editorsCompPtr.GetCount();
	for (int i = 0; i < editorsCount; ++i){
		imod::IModelEditor* editorPtr = m_editorsCompPtr[i];
		if (editorPtr != NULL){
			editorPtr->UpdateModel();
		}
	}
}


void CComposedParamsSetGuiComp::UpdateEditor()
{
	int editorsCount = m_editorsCompPtr.GetCount();
	for (int i = 0; i < editorsCount; ++i){
		imod::IModelEditor* editorPtr = m_editorsCompPtr[i];
		if (editorPtr != NULL){
			editorPtr->UpdateEditor();
		}
	}
}


// reimplemented (imod::IObserver)

bool CComposedParamsSetGuiComp::OnAttached(imod::IModel* modelPtr)
{
	if (!BaseClass::OnAttached(modelPtr)){
		return false;
	}

	iprm::IParamsSet* paramsSetPtr = GetObjectPtr();
	I_ASSERT(paramsSetPtr != NULL);

	int elementsCount = istd::Min(m_observersCompPtr.GetCount(), m_idsAttrPtr.GetCount());
	for (int i = 0; i < elementsCount; ++i){
		const istd::CString& paramId = m_idsAttrPtr[i];

		imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(paramsSetPtr->GetEditableParameter(paramId.ToString()));
		imod::IObserver* observerPtr = m_observersCompPtr[i];

		if ((modelPtr != NULL) && (observerPtr != NULL)){
			modelPtr->AttachObserver(observerPtr);
		}
	}

	return true;
}


bool CComposedParamsSetGuiComp::OnDetached(imod::IModel* modelPtr)
{
	iprm::IParamsSet* paramsSetPtr = GetObjectPtr();
	I_ASSERT(paramsSetPtr != NULL);

	if (!BaseClass::OnDetached(modelPtr)){
		return false;
	}

	int elementsCount = istd::Min(m_observersCompPtr.GetCount(), m_idsAttrPtr.GetCount());
	for (int i = 0; i < elementsCount; ++i){
		const istd::CString& paramId = m_idsAttrPtr[i];

		imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(paramsSetPtr->GetEditableParameter(paramId.ToString()));
		imod::IObserver* observerPtr = m_observersCompPtr[i];

		if ((modelPtr != NULL) && (observerPtr != NULL) && modelPtr->IsAttached(observerPtr)){
			modelPtr->DetachObserver(observerPtr);
		}
	}

	return true;
}


// reimplemented (iqt::CGuiComponentBase)

void CComposedParamsSetGuiComp::OnGuiCreated()
{
	QWidget* widgetPtr = GetQtWidget();
	if (widgetPtr != NULL){
		QLayout* layoutPtr = NULL;
		if (*m_useHorizontalLayoutAttrPtr){
			layoutPtr = new QHBoxLayout(widgetPtr);
		}
		else{
			layoutPtr = new QVBoxLayout(widgetPtr);
		}

		layoutPtr->setMargin(0);

		if (*m_designTypeAttrPtr == 1){
			widgetPtr->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);

			QToolBox* toolBoxPtr = new QToolBox(widgetPtr);
			int elementsCount = m_guisCompPtr.GetCount();
			for (int i = 0; i < elementsCount; ++i){
				iqt::IGuiObject* guiPtr = m_guisCompPtr[i];

				if (guiPtr != NULL){
					QWidget* panelPtr = new QWidget(toolBoxPtr);
					QLayout* panelLayoutPtr = new QVBoxLayout(panelPtr);
					panelLayoutPtr->setContentsMargins(0, 0, 0, 0);
					QString name;
					if (i < m_namesAttrPtr.GetCount()){
						name = iqt::GetQString(m_namesAttrPtr[i]);
					}

					guiPtr->CreateGui(panelPtr);

					toolBoxPtr->addItem(panelPtr, name);

					QSpacerItem* spacerPtr = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

					panelLayoutPtr->addItem(spacerPtr);
				}
			}

			layoutPtr->addWidget(toolBoxPtr);
		}
		else{
			int elementsCount = m_guisCompPtr.GetCount();
			for (int i = 0; i < elementsCount; ++i){
				iqt::IGuiObject* guiPtr = m_guisCompPtr[i];

				if (guiPtr != NULL){
					guiPtr->CreateGui(widgetPtr);
				}
			}
		}
	}

	BaseClass::OnGuiCreated();
}


// reimplemented (iqt2d::ISceneExtender)

void CComposedParamsSetGuiComp::AddItemsToScene(iqt2d::ISceneProvider* providerPtr, int flags)
{
	I_ASSERT(providerPtr != NULL);

	int elementsCount = m_extendersCompPtr.GetCount();
	for (int i = 0; i < elementsCount; ++i){
		iqt2d::ISceneExtender* extenderPtr = m_extendersCompPtr[i];
		if (extenderPtr != NULL){
			extenderPtr->AddItemsToScene(providerPtr, flags);
		}
	}
}


void CComposedParamsSetGuiComp::RemoveItemsFromScene(iqt2d::ISceneProvider* providerPtr)
{
	I_ASSERT(providerPtr != NULL);

	int elementsCount = m_extendersCompPtr.GetCount();
	for (int i = 0; i < elementsCount; ++i){
		iqt2d::ISceneExtender* extenderPtr = m_extendersCompPtr[i];
		if (extenderPtr != NULL){
			extenderPtr->RemoveItemsFromScene(providerPtr);
		}
	}
}


} // namespace iqtprm


