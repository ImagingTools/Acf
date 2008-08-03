#ifndef iqt2d_TObjectShapeBase_included
#define iqt2d_TObjectShapeBase_included


#include "istd/TChangeNotifier.h"

#include "imod/TSingleModelObserverBase.h"

#include "iqt2d/TShapeBase.h"


namespace iqt2d
{


template <class GraphicsItemClass>
class TObjectShapeBase: 
			public TShapeBase<GraphicsItemClass>,
			public imod::TSingleModelObserverBase<i2d::IObject2d>
{
public:
	typedef TShapeBase<GraphicsItemClass> BaseClass;
	typedef imod::TSingleModelObserverBase<i2d::IObject2d> BaseClass2;

	TObjectShapeBase(QGraphicsItem* parentPtr = NULL);

protected:
	// reimplemented (QGraphicsItem) 
	virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

private:
	QPointF m_lastPosition;
};


// public methods

template <class GraphicsItemClass>
TObjectShapeBase<GraphicsItemClass>::TObjectShapeBase(QGraphicsItem* parentPtr)
	:BaseClass(parentPtr)
{
}


// reimplemented (QGraphicsItem) 

template <class GraphicsItemClass>
QVariant TObjectShapeBase<GraphicsItemClass>::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
	if (change == BaseClass::ItemPositionChange){
		i2d::IObject2d* objectPtr = GetObjectPtr();
		if (objectPtr != NULL && change == ItemPositionChange){
			QPointF newPosition = value.toPointF();
			QPointF offset = newPosition - m_lastPosition;

			istd::CChangeNotifier changePtr(objectPtr);

			objectPtr->MoveTo(iqt::GetCVector2d(offset) + objectPtr->GetCenter());

			m_lastPosition = newPosition;

			OnPositionChanged(newPosition);

			return pos();
		}
	}

	return BaseClass::itemChange(change, value);
}


template <class GraphicsItemClass>
void TObjectShapeBase<GraphicsItemClass>::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	BaseClass::mouseReleaseEvent(event);

	m_lastPosition = QPointF(0, 0);
}


} // namespace iqt2d


#endif // !iqt2d_TObjectShapeBase_included


