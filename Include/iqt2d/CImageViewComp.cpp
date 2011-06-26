#include "iqt2d/CImageViewComp.h"


// ACF includes
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsSceneWheelEvent>
#include <QKeyEvent>
#include <QMenu>


namespace iqt2d
{


// public methods

// reimplemented (imod::IObserver)

bool CImageViewComp::OnDetached(imod::IModel* modelPtr)
{
	if (BaseClass::OnDetached(modelPtr)){
		QGraphicsScene* scenePtr = GetScene();
		if (scenePtr != NULL){
			scenePtr->update();
		}

		return true;
	}

	return false;
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CImageViewComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	istd::CIndex2d imageSize = GetSize();
	if (imageSize == istd::CIndex2d::GetZero() || imageSize == istd::CIndex2d::GetInvalid()){
		QGraphicsScene* scenePtr = GetScene();
		if (scenePtr != NULL){
			scenePtr->update();
		}

		return;
	}

	QGraphicsScene* scenePtr = GetScene();
	if (scenePtr != NULL){
		scenePtr->setSceneRect(boundingRect());

		if (m_fitToViewOnChangeAttrPtr.IsValid() && *m_fitToViewOnChangeAttrPtr){
			OnFitToView();
		}

		SetFittedScale(GetFitMode());
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CImageViewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	SetFrameVisible(*m_isFrameVisibleAttrPtr);
	SetPositionMode(*m_imagePositionModeAttrPtr);

	QGraphicsScene* scenePtr = GetScene();
	if (scenePtr != NULL){
		scenePtr->addItem(this);
	}
}


void CImageViewComp::OnGuiDestroyed()
{
	QGraphicsScene* scenePtr = GetScene();
	if (scenePtr != NULL){
		scenePtr->removeItem(this);
	}

	BaseClass::OnGuiDestroyed();
}


} // namespace iqt2d

