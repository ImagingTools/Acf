#ifndef i2d_CPolylineExtractor_included
#define i2d_CPolylineExtractor_included


// ACF includes
#include "i2d/CPolyline.h"
#include "i2d/CVector2d.h"
#include "i2d/CCircle.h"


namespace i2d
{


/**
	Helper class for conversion of common geometrical primitives into a polyline.
*/
class CPolylineExtractor
{
public:
	/**
		\param pointCount Number of nodes to approximate the circle with a 
		closed polyline. Minimum is 2 or 3.
		\param innerFlag Generate polyline that is inner to the circle, so that 
		its nodes lie on a circle, and edges are chords of arcs. In an outer 
		polyline, edges are tangential to the circle and nodes lie around. The 
		exact relation between inner and outer radius is cos(pi/pointCount).
	 */
	static i2d::CPolyline CreatePolyline(const i2d::CCircle& circle, int pointCount, bool clockwiseFlag = false, bool innerFlag = true);
};


} // namespace i2d


#endif // !i2d_CPolylineExtractor_included
