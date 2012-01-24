#ifndef i2d_CPolylineExtractor_included
#define i2d_CPolylineExtractor_included


// ACF includes
#include "i2d/CPolyline.h"
#include "i2d/CVector2d.h"
#include "i2d/CCircle.h"


namespace i2d
{


class CPolylineExtractor
{
public:
	static i2d::CPolyline CreatePolyline(const i2d::CCircle& circle, int pointCount, bool clockwiseFlag = false);
};


} // namespace i2d


#endif // !i2d_CPolylineExtractor_included
