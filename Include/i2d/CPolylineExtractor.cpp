#include "i2d/CPolylineExtractor.h"


namespace i2d
{


i2d::CPolyline CPolylineExtractor::CreatePolyline(const i2d::CCircle& circle, int pointCount, bool clockwiseFlag, bool innerFlag)
{
	i2d::CPolyline retVal;

	const int minPoints = innerFlag ? 3 : 2;

	if (pointCount >= minPoints){
		const i2d::CVector2d& center = circle.GetPosition();
		double radius = circle.GetRadius();

		double step = clockwiseFlag ? I_2PI / pointCount : -I_2PI / pointCount;

		if (!innerFlag){
			radius /= qCos(step / 2);
		}

		double angle = 0;

		for (int i = 0; i < pointCount; ++i, angle += step){
			i2d::CVector2d position(qCos(angle) * radius, qSin(angle) * radius);

			retVal.InsertNode(center + position);
		}

		retVal.SetClosed();
	}

	return retVal;
}


}// namespace i2d


