#include "iqt2d/CGripShape.h"


#include <QPen>
#include <QBrush>


namespace iqt2d
{


// public methods

CGripShape::CGripShape(QGraphicsItem* parentPtr)
	:BaseClass(parentPtr)
{
	BaseClass::setPen(QPen(QBrush(QColor(10, 126, 242, 255)), 0));
	BaseClass::setBrush(QBrush(QColor(10, 126, 242, 50)));
	setRect(-4, -4, 8, 8);

	setFlags(ItemIsMovable);
}


// protected methods

QVariant CGripShape::itemChange(GraphicsItemChange change, const QVariant& value)
{
	if (change == ItemPositionChange) {
		emit PositionChanged(value.toPointF());
	}

	return BaseClass::itemChange(change, value);
}


} // namespace iqt2d


