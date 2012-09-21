#ifndef i2d_CPolygonExtractor_included
#define i2d_CPolygonExtractor_included


// ACF includes
#include "i2d/CPolygon.h"
#include "i2d/CVector2d.h"
#include "i2d/CAnnulus.h"


namespace i2d
{


/**
	Helper class for approximation of some 2D-object by a polygon.
*/
class CPolygonExtractor
{
public:
	/**
		Approximate an annulus region by polygon.
		\param annulus	Annulus object to be approximated.
		\param innerNodesCount	Number of polygon nodes to approximate the inner circle of the annulus.
		\param outerNodesCount	Number of polygon nodes to approximate the outer circle of the annulus.
	*/
	static i2d::CPolygon CreatePolygon(const i2d::CAnnulus& annulus, int innerNodesCount, int outerNodesCount);
};


} // namespace i2d


#endif // !i2d_CPolygonExtractor_included

