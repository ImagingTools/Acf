#include "icmpstr/CConnectorShape.h"


// STL includes
#include <cmath>

// Qt includes
#include <QPainter>

#include "i2d/CVector2d.h"

#include "iqt/iqt.h"

#include "icmpstr/CComponentSceneItem.h"
#include "icmpstr/CRegistryView.h"


// public methods

namespace icmpstr
{


CConnectorShape::CConnectorShape(
			int connectFlags,
			QGraphicsItem* parent)
:	BaseClass(parent),
	m_connectFlags(connectFlags),
	m_beginPosition(0, 0),
	m_beginDirection(0, 0),
	m_endPosition(0, 0),
	m_endDirection(0, 0)
{
	setAcceptedMouseButtons(0);
	setAcceptsHoverEvents(true);

	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	setZValue(1);
}


// reimplemented (QGraphicsItem)

int CConnectorShape::type() const
{ 
	return Type; 
}


QPainterPath CConnectorShape::shape() const
{
	QPainterPath path;
	path.addRect(boundingRect());

	return path;
}


// public slots

void CConnectorShape::SetBeginPoint(const QPointF& position, const QPointF& direction)
{
	m_beginPosition = position;
	m_beginDirection = direction;

	Adjust();

	update();
}


void CConnectorShape::SetEndPoint(const QPointF& position, const QPointF& direction)
{
	m_endPosition = position;
	m_endDirection = direction;

	Adjust();

	update();
}


// protected members

void CConnectorShape::Adjust()
{
	prepareGeometryChange();

	m_connectionLine.clear();

	m_connectionLine.append(m_beginPosition);

	i2d::CVector2d firstPoint = iqt::GetCVector2d(m_beginPosition);
	i2d::CVector2d firstDirection = iqt::GetCVector2d(m_beginDirection);
	i2d::CVector2d lastPoint = iqt::GetCVector2d(m_endPosition);
	i2d::CVector2d lastDirection = iqt::GetCVector2d(m_endDirection);

	i2d::CVector2d delta = lastPoint - firstPoint;

	double directionProduct = firstDirection.GetDotProduct(lastDirection);

	if (directionProduct > I_BIG_EPSILON){	// the same direction
		i2d::CVector2d commonDirection = (firstDirection + lastDirection).GetNormalized();
		double firstProjection = commonDirection.GetDotProduct(firstPoint);
		double lastProjection = commonDirection.GetDotProduct(lastPoint);

		if (firstProjection > lastProjection){
			m_connectionLine.append(iqt::GetQPointF(firstPoint + commonDirection * GP_BRIDGE_LENGTH));
			m_connectionLine.append(iqt::GetQPointF(lastPoint + commonDirection * (GP_BRIDGE_LENGTH + firstProjection - lastProjection)));
		}
		else{
			m_connectionLine.append(iqt::GetQPointF(firstPoint + commonDirection * (GP_BRIDGE_LENGTH + lastProjection - firstProjection)));
			m_connectionLine.append(iqt::GetQPointF(lastPoint + commonDirection * GP_BRIDGE_LENGTH));
		}
	}
	else if (directionProduct < I_BIG_EPSILON){
		i2d::CVector2d commonDirection = (firstDirection + lastDirection).GetNormalized();
		double firstProjection = commonDirection.GetDotProduct(firstPoint);
		double lastProjection = commonDirection.GetDotProduct(lastPoint);
		double midProjection = (firstProjection + lastProjection) * 0.5;

		m_connectionLine.append(iqt::GetQPointF(firstPoint + commonDirection * (midProjection - firstProjection)));
		m_connectionLine.append(iqt::GetQPointF(lastPoint + commonDirection * (midProjection - lastProjection)));
	}
	else{
		i2d::CLine2d firstLine(firstPoint, firstPoint + firstDirection);
		i2d::CLine2d lastLine(lastPoint, lastPoint + lastDirection);

		i2d::CVector2d extIntersection;

		if (firstLine.GetExtendedIntersection(lastLine, extIntersection)){
			m_connectionLine.append(iqt::GetQPointF(extIntersection));
		}
	}

	m_connectionLine.append(m_endPosition);
}


// reimplemented (QGraphicsItem)

QRectF CConnectorShape::boundingRect() const
{
	double penWidth = 1.0;

	QRectF rect = m_connectionLine.boundingRect();

	return rect.adjusted(-penWidth, -penWidth, penWidth, penWidth);
}


void CConnectorShape::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	// drawing:
	painter->save();

	painter->setPen(QPen(
				((m_connectFlags & CF_FACTORY) != 0)? Qt::darkYellow: Qt::darkBlue,
				1,
				((m_connectFlags & CF_EMBEDDED) != 0)? Qt::DashDotLine: Qt::SolidLine,
				Qt::RoundCap,
				Qt::RoundJoin));

	painter->setRenderHints(QPainter::Antialiasing, false);

	painter->drawPolyline(m_connectionLine);

	painter->restore();
}


void CConnectorShape::hoverEnterEvent(QGraphicsSceneHoverEvent* eventPtr)
{
	setSelected(true);
	
	update();

	BaseClass::hoverEnterEvent(eventPtr);
}


void CConnectorShape::hoverLeaveEvent(QGraphicsSceneHoverEvent* eventPtr)
{
	setSelected(false);

	update();

	BaseClass::hoverLeaveEvent(eventPtr);
}


} // namespace icmpstr


