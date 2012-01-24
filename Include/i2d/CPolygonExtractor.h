#ifndef i2d_CPolygonExtractor_included
#define i2d_CPolygonExtractor_included

// ACF includes
#include "i2d/CPolygon.h"
#include "i2d/CVector2d.h"
#include "i2d/CAnnulus.h"


namespace i2d
{


class CPolygonExtractor
{
public:
	static i2d::CPolygon CreatePolygon(const i2d::CAnnulus& annulus, int innerNodesCount, int outerNodesCount);
};


} // namespace i2d


#endif // !i2d_CPolygonExtractor_included

