#ifndef i2d_CCircle_included
#define	i2d_CCircle_included


#include "i2d/IObject2d.h"

#include "imath/CVector2d.h"


namespace i2d
{	


class CCircle: virtual public i2d::IObject2d
{
public:
	CCircle();
	CCircle(double radius, const CPoint& center);

	double GetRadius() const;

	// reimplemented (i2d::IObject2d)
	virtual CPoint GetCenter() const;
	virtual CRectangle GetBoundingBox() const;

protected:
	double m_radius;
	CPoint m_center;
};


} // namespace i2d


#endif // !i2d_CCircle_included

