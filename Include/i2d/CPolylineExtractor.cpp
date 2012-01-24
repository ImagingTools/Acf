#include "i2d/CPolylineExtractor.h"


namespace i2d
{


i2d::CPolyline CPolylineExtractor::CreatePolyline(const i2d::CCircle& circle, int pointCount, bool clockwiseFlag)
{
	i2d::CPolyline retVal;

	const i2d::CVector2d& center = circle.GetPosition();
	double radius = circle.GetRadius();

	if (pointCount >= 2){
		double step = clockwiseFlag ? I_2PI / pointCount : -I_2PI / pointCount;
		double angle = 0;

		for (int i = 0; i < pointCount; ++i, angle += step){
			i2d::CVector2d position(cos(angle) * radius, sin(angle) * radius);

			retVal.InsertNode(center + position);
		}

		retVal.SetClosed();
	}

	return retVal;
}


}// namespace i2d


