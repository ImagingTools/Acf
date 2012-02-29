#include "iview/CInteractivePolylineSegmentShadeShape.h"


// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include "iqt/iqt.h"


namespace iview
{


// reimplemented (imod::IObserver)

bool CInteractivePolylineSegmentShadeShape::OnAttached(imod::IModel* modelPtr)
{
	if (dynamic_cast<Model*>(modelPtr) != NULL){
		return BaseClass::OnAttached(modelPtr);
	}
	else{
		return false;
	}
}


// protected methods

// reimplemented (iview::CInteractivePolygonShape)

void CInteractivePolylineSegmentShadeShape::DrawCurve(QPainter& drawContext) const
{
	I_ASSERT(IsDisplayConnected());

	const Model* polylinePtr = dynamic_cast<const Model*>(GetModelPtr());
	if (polylinePtr != NULL){
		const iview::CScreenTransform& transform = GetLogToScreenTransform();
        const IColorShema& colorShema = GetColorShema();

		int nodesCount = polylinePtr->GetNodesCount();
		if (nodesCount > 0){
			int secondPointIndex;
			int pointIndex;
			istd::CIndex2d firstPoint;
			int firstPointIndex;

			if (polylinePtr->IsClosed()){
				firstPointIndex = nodesCount - 1;
				secondPointIndex = 0;
			}
			else{
				firstPointIndex = 0;
				secondPointIndex = 1;
			}
			firstPoint = transform.GetScreenPosition(polylinePtr->GetNode(firstPointIndex));

			if (IsOrientationVisible() && IsSelected()){
				const QPen& darkPen = colorShema.GetPen(IColorShema::SP_ORIENT_DARK);
				const QPen& brightPen = colorShema.GetPen(IColorShema::SP_ORIENT_BRIGHT);

				istd::CIndex2d point1 = firstPoint;
				for (pointIndex = secondPointIndex; pointIndex < nodesCount; ++pointIndex){
					istd::CIndex2d point2 = transform.GetScreenPosition(polylinePtr->GetNode(pointIndex));
					istd::CIndex2d delta(point2.GetX() - point1.GetX(), point2.GetY() - point1.GetY());
					if (::abs(delta.GetY()) > ::abs(delta.GetX())){
						if (delta.GetY() > 0){
							drawContext.save();
							drawContext.setPen(darkPen);
							drawContext.drawLine(QPoint(point1.GetX() - 1, point1.GetY()), QPoint(point2.GetX() - 1, point2.GetY()));
							drawContext.restore();
	
							drawContext.save();
							drawContext.setPen(brightPen);
							drawContext.drawLine(QPoint(point1.GetX() + 1, point1.GetY()), QPoint(point2.GetX() + 1, point2.GetY()));
							drawContext.restore();
						}
						else{
							drawContext.save();
							drawContext.setPen(darkPen);
							drawContext.drawLine(QPoint(point1.GetX() + 1, point1.GetY()), QPoint(point2.GetX() + 1, point2.GetY()));
							drawContext.restore();

							drawContext.save();
							drawContext.setPen(brightPen);
							drawContext.drawLine(QPoint(point1.GetX() - 1, point1.GetY()), QPoint(point2.GetX() - 1, point2.GetY()));
							drawContext.restore();
						}
					}
					else{
						if (delta.GetX() > 0){
							drawContext.save();
							drawContext.setPen(darkPen);
							drawContext.drawLine(QPoint(point1.GetX(), point1.GetY() + 1), QPoint(point2.GetX(), point2.GetY() + 1));
							drawContext.restore();

							drawContext.save();
							drawContext.setPen(brightPen);
							drawContext.drawLine(QPoint(point1.GetX(), point1.GetY() - 1), QPoint(point2.GetX(), point2.GetY() - 1));
							drawContext.restore();
						}
						else{
							drawContext.save();
							drawContext.setPen(darkPen);
							drawContext.drawLine(QPoint(point1.GetX(), point1.GetY() - 1), QPoint(point2.GetX(), point2.GetY() - 1));
							drawContext.restore();

							drawContext.save();
							drawContext.setPen(brightPen);
							drawContext.drawLine(QPoint(point1.GetX(), point1.GetY() + 1), QPoint(point2.GetX(), point2.GetY() + 1));
							drawContext.restore();
						}
					}
					point1 = point2;
				}
			}

			QPoint point1 = iqt::GetQPoint(firstPoint);
			const IColorShema::PensPtrSet& pensPtrSet = colorShema.GetPensPtrSet(IColorShema::SPS_NORMAL);
			int pensSetCount = int(pensPtrSet.size());
			for (pointIndex = secondPointIndex; pointIndex < nodesCount; ++pointIndex){
				double brightness = polylinePtr->GetSegmentData(firstPointIndex);
				int penIndex = int(brightness * pensSetCount);
				if (penIndex < 0){
					penIndex = 0;
				}
				else if (penIndex >= pensSetCount){
					penIndex = pensSetCount - 1;
				}
				const QPen* penPtr = pensPtrSet[penIndex];
				I_ASSERT(penPtr != NULL);

				QPoint point2 = iqt::GetQPoint(transform.GetScreenPosition(polylinePtr->GetNode(pointIndex)));

				drawContext.save();
				drawContext.setPen(*penPtr);
				drawContext.drawLine(point1, point2);
				drawContext.restore();

				++firstPointIndex;
				point1 = point2;
			}
		}
	}
}


} // namespace iview


