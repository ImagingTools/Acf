#include "CAffineTransformation2dShape.h"
#include <i2d/CAffineTransformation2d.h>
#include "IColorSchema.h"
#include "CConsoleBase.h"

// Qt includes
#include <QtGui/QPainter>

namespace iview
{


CAffineTransformation2dShape::CAffineTransformation2dShape(void)
: m_activeControlPoints(ALL_POINTS), m_currentPoint(NO_POINT)
{
}


void CAffineTransformation2dShape::SetActiveControlPoints(ControlPoint points)
{
	m_activeControlPoints = points;
}


namespace
{


/** 
	Structure representing a set of 5 control points of the shape
 */
struct CControlPoints
{


	/** Generate transformed points using given transformation. 
	 
		If there is no transformation, generate default Cartesian coordinate 
		system corner points: begin, X axis end and Y axis end. 
	 */
	CControlPoints(const i2d::CAffineTransformation2d * transformationPtr = NULL)
	{
		// points delimiting the original coordinate system
		points[0] = i2d::CVector2d(0, 1);
		points[1] = i2d::CVector2d(1, 1);
		points[2] = i2d::CVector2d(0, 0);

		if (transformationPtr != NULL){
			points[0] = transformationPtr->GetValueAt(points[0]);
			points[1] = transformationPtr->GetValueAt(points[1]);
			points[2] = transformationPtr->GetValueAt(points[2]);
			points[3] = points[3] + points[2] - points[1];
			points[4] = (points[4] + points[1]) / 2;
		}
	}


	/** 
		Transform control points to screen coordinates 
	 */
	CControlPoints ToScreen(const iview::CScreenTransform & transform)
	{
		CControlPoints result;
		for (int i = 0; i < 5; i++){
			result.points[i] = transform.GetApply(points[i]);
		}

		return result;
	}


	void ResetTransformation(i2d::CAffineTransformation2d * transformationPtr)
	{
		i2d::CVector2d translation = points[0];
		double angle = atan2(points[1].GetY(), points[1].GetX());
		i2d::CVector2d scale(points[1].GetDistance(points[0]), points[2].GetDistance(points[0]));

		transformationPtr->Reset(translation, angle, scale);
	}


	i2d::CVector2d& operator[](int index)
	{
		return points[index];
	}

	i2d::CVector2d points[5];

}; // struct CControlPoints


} // namespace


bool CAffineTransformation2dShape::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	I_ASSERT(IsDisplayConnected());

	const i2d::CAffineTransformation2d* transformationPtr =
			dynamic_cast<const i2d::CAffineTransformation2d*>(GetModelPtr());
	if (transformationPtr == NULL){
		return false;
	}

	if (downFlag){
		// use model transformation to transform original points
		CControlPoints points(transformationPtr);

		const iview::CScreenTransform& transform = GetLogToScreenTransform();

		CControlPoints screenPoints = points.ToScreen(transform);


		const double pointRadius = 2.5; // pixels
		i2d::CVector2d pos(position.GetX(), position.GetY());

		m_currentPoint = NO_POINT;
		// check whether any of the points or its surrounding was clicked
		for (int i = 0; i < 5; i++){
			ControlPoint point = (ControlPoint)(i + 1);
			if ((m_activeControlPoints & point) != NO_POINT && screenPoints[i].GetDistance(pos) < pointRadius){
				m_currentPoint = point;
				BeginModelChanges();
				return true;
			}
		}
	}
	else{
		m_currentPoint = NO_POINT;
		EndModelChanges();
		return true;
	}

	return false;
}


bool CAffineTransformation2dShape::OnMouseMove(istd::CIndex2d position)
{
	const iview::CScreenTransform& transform = GetLogToScreenTransform();

	i2d::CAffineTransformation2d* transformationPtr = dynamic_cast<i2d::CAffineTransformation2d*>(GetModelPtr());

	if (transformationPtr != NULL && m_currentPoint != NO_POINT && m_currentPoint <= POINT5 && (m_activeControlPoints & m_currentPoint) != NO_POINT){
		const i2d::CVector2d& cp = transform.GetClientPosition(position);

		CControlPoints points(transformationPtr);

		points[m_currentPoint - 1].SetX(cp.GetX());
		points[m_currentPoint - 1].SetY(cp.GetY());

		points.ResetTransformation(transformationPtr);
		UpdateModelChanges();

		return true;
	}

	return false;
}


void CAffineTransformation2dShape::Draw(QPainter& drawContext) const
{
	I_ASSERT(IsDisplayConnected());

	QPen baseSystemPen(QColor("cyan"));
	QPen transformedSystemPen(QColor("blue"));
	QPen transformationOffsetsPen(QColor("brown"));
	transformationOffsetsPen.setStyle(Qt::DotLine);
	QPen additionalLinesPen(QColor("blue"));
	additionalLinesPen.setStyle(Qt::DotLine);

	drawContext.save();


	i2d::CAffineTransformation2d* transformationPtr = dynamic_cast<i2d::CAffineTransformation2d*>(GetModelPtr());
	const iview::CScreenTransform& transform = GetLogToScreenTransform();

	i2d::CVector2d arrowPoint1(-0.02, 0.05);
	i2d::CVector2d arrowPoint2(0.02, 0.05);
	i2d::CVector2d arrowPoint3(1 - 0.05, 1 - 0.02);
	i2d::CVector2d arrowPoint4(1 - 0.05, 1 + 0.02);
	CControlPoints controlPoints;
	CControlPoints screenPoints = controlPoints.ToScreen(transform);

	// draw base coordinate system
	drawContext.setPen(baseSystemPen);
	i2d::CVector2d screenArrowPoint1 = transform.GetApply(arrowPoint1);
	i2d::CVector2d screenArrowPoint2 = transform.GetApply(arrowPoint2);
	drawContext.drawLine(screenPoints[0].GetX(), screenPoints[0].GetY(), screenPoints[1].GetX(), screenPoints[1].GetY());
	QPointF points[3];
	points[0] = QPointF(screenPoints[1].GetX(), screenPoints[1].GetY());
	points[1] = QPointF(screenArrowPoint1.GetX(), screenArrowPoint1.GetY());
	points[2] = QPointF(screenArrowPoint2.GetX(), screenArrowPoint2.GetY());
	drawContext.drawPolygon(points, 3);
	drawContext.drawLine(screenPoints[0].GetX(), screenPoints[0].GetY(), screenPoints[2].GetX(), screenPoints[2].GetY());
	i2d::CVector2d screenArrowPoint3 = transform.GetApply(arrowPoint3);
	i2d::CVector2d screenArrowPoint4 = transform.GetApply(arrowPoint4);
	points[0] = QPointF(screenPoints[2].GetX(), screenPoints[2].GetY());
	points[1] = QPointF(screenArrowPoint3.GetX(), screenArrowPoint3.GetY());
	points[2] = QPointF(screenArrowPoint4.GetX(), screenArrowPoint4.GetY());
	drawContext.drawPolygon(points, 3);

	// draw transformed coordinate system
	drawContext.setPen(transformedSystemPen);
	controlPoints = CControlPoints(transformationPtr);
	CControlPoints transformedScreenPoints = controlPoints.ToScreen(transform);
	arrowPoint1 = transformationPtr->GetValueAt(arrowPoint1);
	arrowPoint2 = transformationPtr->GetValueAt(arrowPoint2);
	arrowPoint3 = transformationPtr->GetValueAt(arrowPoint3);
	arrowPoint4 = transformationPtr->GetValueAt(arrowPoint4);
	drawContext.drawLine(transformedScreenPoints[0].GetX(), transformedScreenPoints[0].GetY(), transformedScreenPoints[1].GetX(), transformedScreenPoints[1].GetY());
	screenArrowPoint1 = transform.GetApply(arrowPoint1);
	screenArrowPoint2 = transform.GetApply(arrowPoint2);
	points[0] = QPointF(transformedScreenPoints[1].GetX(), transformedScreenPoints[1].GetY());
	points[1] = QPointF(screenArrowPoint1.GetX(), screenArrowPoint1.GetY());
	points[2] = QPointF(screenArrowPoint2.GetX(), screenArrowPoint2.GetY());
	drawContext.drawPolygon(points, 3);
	drawContext.drawLine(transformedScreenPoints[0].GetX(), transformedScreenPoints[0].GetY(), transformedScreenPoints[2].GetX(), transformedScreenPoints[2].GetY());
	screenArrowPoint3 = transform.GetApply(arrowPoint3);
	screenArrowPoint4 = transform.GetApply(arrowPoint4);
	points[0] = QPointF(transformedScreenPoints[2].GetX(), transformedScreenPoints[2].GetY());
	points[1] = QPointF(screenArrowPoint3.GetX(), screenArrowPoint3.GetY());
	points[2] = QPointF(screenArrowPoint4.GetX(), screenArrowPoint4.GetY());
	drawContext.drawPolygon(points, 3);

	// draw lines connecting base system corner points with their transformed counterparts
	drawContext.setPen(transformationOffsetsPen);
	drawContext.drawLine(screenPoints[0].GetX(), screenPoints[0].GetY(), transformedScreenPoints[0].GetX(), transformedScreenPoints[0].GetY());
	drawContext.drawLine(screenPoints[1].GetX(), screenPoints[1].GetY(), transformedScreenPoints[1].GetX(), transformedScreenPoints[1].GetY());
	drawContext.drawLine(screenPoints[2].GetX(), screenPoints[2].GetY(), transformedScreenPoints[2].GetX(), transformedScreenPoints[2].GetY());

	// draw lines connecting transformed corner points with point 4 (and 5)
	drawContext.setPen(additionalLinesPen);
	drawContext.drawLine(transformedScreenPoints[0].GetX(), transformedScreenPoints[0].GetY(), transformedScreenPoints[3].GetX(), transformedScreenPoints[3].GetY());
	drawContext.drawLine(transformedScreenPoints[1].GetX(), transformedScreenPoints[1].GetY(), transformedScreenPoints[3].GetX(), transformedScreenPoints[3].GetY());
	drawContext.drawLine(transformedScreenPoints[2].GetX(), transformedScreenPoints[2].GetY(), transformedScreenPoints[3].GetX(), transformedScreenPoints[3].GetY());

	// draw full and empty circles representing control points
	for (int i = 1; i <= 5; i++){
		if ((m_activeControlPoints & i) != NO_POINT){
			drawContext.
		}
		else{
		}
	}

	drawContext.restore();
}


bool CAffineTransformation2dShape::OnAttached(imod::IModel* modelPtr)
{
	I_ASSERT(dynamic_cast<i2d::CAffineTransformation2d*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


ITouchable::TouchState CAffineTransformation2dShape::IsTouched(istd::CIndex2d position) const
{
	const i2d::CAffineTransformation2d* transformationPtr = dynamic_cast<const i2d::CAffineTransformation2d*>(GetModelPtr());
	if (IsDisplayConnected() && (transformationPtr != NULL)){
		const iview::CScreenTransform& transform = GetLogToScreenTransform();
		i2d::CVector2d cp = transform.GetClientPosition(position);

		const double pointRadius = 2.5; // pixels

		CControlPoints movablePoints(transformationPtr);
		for (int i = 1; i <= 5; i++){
			if ((m_activeControlPoints & i) == NO_POINT){
				continue;
			}

			if (movablePoints[i - 1].GetDistance(cp) < pointRadius){
				return TS_DRAGGABLE;
			}
		}
	}

	return TS_NONE;
}


i2d::CRect CAffineTransformation2dShape::CalcBoundingBox() const
{
	I_ASSERT(IsDisplayConnected());

	i2d::CAffineTransformation2d* transformationPtr = dynamic_cast<i2d::CAffineTransformation2d*>(GetModelPtr());
	const iview::CScreenTransform& transform = GetLogToScreenTransform();

	CControlPoints screenPoints = CControlPoints().ToScreen(transform);
	CControlPoints transformedScreenPoints = CControlPoints(transformationPtr).ToScreen(transform);

	i2d::CRect result(screenPoints[0].GetX(), screenPoints[0].GetY(), screenPoints[0].GetX(), screenPoints[0].GetY());

	for (int i = 1; i < 5; i++){
		i2d::CRect rect(screenPoints[i].GetX(), screenPoints[i].GetY(), screenPoints[i].GetX(), screenPoints[i].GetY());
		result.Union(rect);
	}

	for (int i = 0; i < 5; i++){
		i2d::CRect rect(transformedScreenPoints[i].GetX(), transformedScreenPoints[i].GetY(), transformedScreenPoints[i].GetX(), transformedScreenPoints[i].GetY());
		result.Union(rect);
	}

	double pointMaxRadius = 0.05 * transform.GetDeformMatrix().GetApproxScale();

	result.SetLeft(result.GetLeft() - pointMaxRadius);
	result.SetRight(result.GetRight() + pointMaxRadius);
	result.SetTop(result.GetRight() - pointMaxRadius);
	result.SetBottom(result.GetRight() + pointMaxRadius);

	return result;
}


}

