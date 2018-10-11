#include <iview/CToolsViewLayer.h>


// Qt includes
#include <QtCore/QMapIterator>
#include <QtGui/QPainter>

// ACF includes
#include <iqt/iqt.h>
#include <iview/IInteractiveShape.h>
#include <iview/IShapeView.h>
#include <iview/IDraggable.h>
#include <iview/IShapeView.h>
#include <iview/IViewEventObserver.h>
#include <iview/CScreenTransform.h>


namespace iview
{


// public methods

// reimplemented (iview::ISelectableLayer)

bool CToolsViewLayer::ConnectInteractiveShape(IInteractiveShape* shapePtr)
{
	return BaseClass::ConnectShape(shapePtr);
}


int CToolsViewLayer::GetUnselectedShapesCount() const
{
	return BaseClass::GetShapesCount();
}


void CToolsViewLayer::DrawFocusedShape(QPainter& drawContext)
{
	BaseClass::DrawShapes(drawContext);
}


bool CToolsViewLayer::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	QMapIterator<IShape*, i2d::CRect> inactiveIter(m_shapes);
	inactiveIter.toBack();
	while (inactiveIter.hasPrevious()){
		inactiveIter.previous();

		iview::IInteractiveShape* uiShapePtr = dynamic_cast<iview::IInteractiveShape*>(inactiveIter.key());

		if (uiShapePtr->OnMouseButton(position, buttonType, downFlag)){
			return true;
		}
	}

	return false;
}


bool CToolsViewLayer::OnFocusedMouseButton(istd::CIndex2d /*position*/, Qt::MouseButton /*buttonType*/, bool /*downFlag*/)
{
	return false;
}


bool CToolsViewLayer::OnFocusedMouseMove(istd::CIndex2d position)
{
	QMapIterator<IShape*, i2d::CRect> inactiveIter(m_shapes);
	inactiveIter.toBack();
	while (inactiveIter.hasPrevious()){
		inactiveIter.previous();

		iview::IInteractiveShape* uiShapePtr = dynamic_cast<iview::IInteractiveShape*>(inactiveIter.key());

		if (uiShapePtr->OnMouseMove(position)){
			return true;
		}
	}

	return false;
}


ITouchable::TouchState CToolsViewLayer::IsTouched(istd::CIndex2d position, IInteractiveShape** shapePtrPtr) const
{
	QMapIterator<IShape*, i2d::CRect> inactiveIter(m_shapes);
	inactiveIter.toBack();
	while (inactiveIter.hasPrevious()){
		inactiveIter.previous();
		iview::IInteractiveShape* uiShapePtr = dynamic_cast<iview::IInteractiveShape*>(inactiveIter.key());
		if (uiShapePtr == NULL){
			continue;
		}

		ITouchable::TouchState touchState = uiShapePtr->IsTouched(position);
		if (touchState != TS_NONE){
			if (shapePtrPtr != NULL){
				*shapePtrPtr = uiShapePtr;
			}
			return touchState;
		}	
	}

	return IInteractiveShape::TS_NONE;
}


// reimplemented (iview::ISelectable)

int CToolsViewLayer::GetSelectedShapesCount() const
{
	return 0;
}


void CToolsViewLayer::InsertSelectedShapes(SelectedShapes& result) const
{
	for (ShapeMap::ConstIterator iter = m_shapes.begin(); iter != m_shapes.end(); ++iter){
		IInteractiveShape* uiShape = dynamic_cast<IInteractiveShape*>(iter.key());
		if (uiShape != NULL){
			result.insert(uiShape);
		}
	}
}


void CToolsViewLayer::DeselectAllShapes()
{
}


int CToolsViewLayer::GetKeysState() const
{
	const IShapeView* viewPtr = GetViewPtr();
	Q_ASSERT(viewPtr != NULL);

	return viewPtr->GetKeysState();
}


int CToolsViewLayer::GetEditMode() const
{
	const IShapeView* viewPtr = GetViewPtr();
	Q_ASSERT(viewPtr != NULL);

	return viewPtr->GetEditMode();
}


void CToolsViewLayer::OnShapeFocused(IInteractiveShape* shapePtr)
{
	Q_ASSERT(shapePtr != NULL);
	IShapeView* viewPtr = GetViewPtr();
	Q_ASSERT(viewPtr != NULL);
	viewPtr->OnLayerShapeFocused(shapePtr, this);
}


void CToolsViewLayer::OnShapeDefocused(IInteractiveShape* shapePtr)
{
	Q_ASSERT(shapePtr != NULL);
	IShapeView* viewPtr = GetViewPtr();
	Q_ASSERT(viewPtr != NULL);
	viewPtr->OnLayerShapeDefocused(shapePtr, this);
}


void CToolsViewLayer::OnShapeSelected(IInteractiveShape& /*shape*/, bool /*state*/)
{
}

// reimplemented (iview::ITouchable)

ITouchable::TouchState CToolsViewLayer::IsTouched(istd::CIndex2d position) const
{
	return BaseClass::IsTouched(position);
}

// reimplemented (iview::IDraggable)

void CToolsViewLayer::BeginDrag(const istd::CIndex2d& position)
{
	for (ShapeMap::iterator iter = m_shapes.begin(); iter != m_shapes.end(); ++iter){
		IDraggable* draggablePtr = dynamic_cast<IDraggable*>(iter.key());
		if ((draggablePtr != NULL) && draggablePtr->IsDraggable()){
			draggablePtr->BeginDrag(position);
		}
	}
}


void CToolsViewLayer::SetDragPosition(const istd::CIndex2d& position)
{
	for (ShapeMap::iterator iter = m_shapes.begin(); iter != m_shapes.end(); ++iter){
		IDraggable* draggablePtr = dynamic_cast<IDraggable*>(iter.key());
		if ((draggablePtr != NULL) && draggablePtr->IsDraggable()){
			draggablePtr->SetDragPosition(position);
		}
	}
}


void CToolsViewLayer::EndDrag()
{
	QList<IShape*> keys  = m_shapes.keys();
	for (int keyIndex = 0; keyIndex < keys.size(); ++keyIndex){
		IDraggable* draggablePtr = dynamic_cast<IDraggable*>(keys[keyIndex]);
		if ((draggablePtr != NULL) && draggablePtr->IsDraggable()){
			draggablePtr->EndDrag();
		}
	}
}


bool CToolsViewLayer::IsDraggable() const
{
	return !m_shapes.isEmpty();
}


} // namespace iview


