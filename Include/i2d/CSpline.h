#ifndef i2d_CSpline_included
#define i2d_CSpline_included


// ACF includes
#include "i2d/CPolyline.h"
#include "i2d/CSplineSegment.h"


// forward declarations
namespace i2d{
	class CSpline;
} // namespace i2d


namespace i2d
{


class CSpline: public i2d::CPolyline
{
public:
	typedef i2d::CPolyline BaseClass;

	CSpline();

	/**	Get count of segments.
	 */
	virtual int GetSegmentCount() const;
	/**	Get segment in polynomial.
	 *		@param	index	an index of segment.
	 */
	virtual const CSplineSegment& GetSplineSegment(int index) const;

protected:
	void CalcAllSegments() const;
	
	// reimplemented (i2d::CPolygon)
	virtual void SetInconsistent();

private:
	static void SolveOpen(double* d,int count);

	mutable ::std::vector<CSplineSegment> m_segments;
	mutable bool m_isInvalidated;
};


} // namespace i2d


#endif // !i2d_CSpline_included

