#include "iqt2d/CLine2dShape.h"


#include <QStyleOptionGraphicsItem>
#include <QPainter>

#include "iqt/IQImageProvider.h"


namespace iqt2d
{


// public methods

// reimplemented (imod::IObserver)

void CLine2dShape::AfterUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	update();
}


// reimplemented (QGraphicsRectItem)

QRectF CLine2dShape::boundingRect() const
{
	QRectF retVal;

	const i2d::CLine2d* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		retVal = iqt::GetQRectF(objectPtr->GetBoundingBox());
	}

	return retVal;
}


void CLine2dShape::paint(QPainter* p, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/)
{
	I_ASSERT(p != NULL);

	const i2d::CLine2d* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		p->drawLine(iqt::GetQPointF(objectPtr->GetPoint1()), iqt::GetQPointF(objectPtr->GetPoint2()));
	}
}


} // namespace iqt2d


