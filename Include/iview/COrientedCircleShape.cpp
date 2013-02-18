// copied from CCircleShape

#include "iview/COrientedCircleShape.h"


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include "imod/IModel.h"
#include "i2d/COrientedCircle.h"
#include "iview/IColorSchema.h"
#include "iview/CScreenTransform.h"


namespace iview
{


COrientedCircleShape::COrientedCircleShape()
{
	m_isEditableRadius = true;
	m_isCenterVisible = true;
}


void COrientedCircleShape::Draw(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	BaseClass::Draw(drawContext);

	const imod::IModel* modelPtr = GetModelPtr();
	const i2d::COrientedCircle& circle = *dynamic_cast<const i2d::COrientedCircle*>(modelPtr);
	Q_ASSERT(&circle != NULL);

	const i2d::ICalibration2d* calibrationPtr = circle.GetCalibration();

	const i2d::CVector2d& center = circle.GetCenter();
	double radius = circle.GetRadius();

	const IColorSchema& colorSchema = GetColorSchema();

	// draw orientation markers as in a polyline
	const QPen& darkPen = colorSchema.GetPen(IColorSchema::SP_ORIENT_DARK);
	const QPen& brightPen = colorSchema.GetPen(IColorSchema::SP_ORIENT_BRIGHT);

	// reduce line opacity for the pens; the border is only used to increase visibility on black/white backgrounds
	QColor brightColor = brightPen.color();
	QBrush brightBrush(brightColor);
	brightColor.setAlphaF(0.25);
	QPen softBrightPen(brightColor);

	QColor darkColor = darkPen.color();
	QBrush darkBrush(darkColor);
	darkColor.setAlphaF(0.25);
	QPen softDarkPen(darkColor);

	const int nodesCount = 8;

	const i2d::CAffine2d& transform = GetViewToScreenTransform();
	double viewScale = transform.GetDeformMatrix().GetApproxScale();

	for (int pointIndex = 0; pointIndex < nodesCount; ++pointIndex){
		double angle = pointIndex * I_2PI / nodesCount;

		i2d::CVector2d vector;
		vector.Init(angle, radius);

		i2d::CVector2d tangent;
		if (circle.IsOrientedOutside()){
			tangent = vector.GetOrthogonal() * (I_PI / nodesCount);
		}
		else{
			tangent = vector.GetOrthogonal() * (-I_PI / nodesCount);
		}

		i2d::CVector2d segmentCenter = center + vector;

		i2d::CLine2d segmentLine(
					GetScreenPosition(segmentCenter - tangent, calibrationPtr),
					GetScreenPosition(segmentCenter + tangent, calibrationPtr));

		DrawOrientationMarker(
					drawContext,
					softBrightPen, darkBrush,
					softDarkPen, brightBrush,
					segmentLine,
					viewScale);
	}
}


bool COrientedCircleShape::OnAttached(imod::IModel* modelPtr)
{
	Q_ASSERT(dynamic_cast<i2d::COrientedCircle*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


i2d::CRect COrientedCircleShape::CalcBoundingBox() const
{
	i2d::CRect result = BaseClass::CalcBoundingBox();
	const iview::CScreenTransform& transform = GetLogToScreenTransform();
	const i2d::CMatrix2d& deform = transform.GetDeformMatrix();
	double directionIndicatorWidth = 10 * deform.GetApproxScale();
	result.SetLeft(result.GetLeft() - directionIndicatorWidth);
	result.SetRight(result.GetRight() + directionIndicatorWidth);
	result.SetTop(result.GetTop() - directionIndicatorWidth);
	result.SetBottom(result.GetBottom() + directionIndicatorWidth);

	return result;
}

} // namespace iview


