#ifndef i2d_IObject2d_included
#define	i2d_IObject2d_included


#include "imath/imath.h"

#include "istd/IPolymorphic.h"


namespace imath
{
	class CVector2d;
}


namespace i2d
{	


class CRectangle;


/**
	Common interface for describing the 2D-objects.
*/
class IObject2d: virtual public istd::IPolymorphic
{
public:
	typedef imath::CVector2d CPoint;

	/**
		Returns center of this 2D-object.
	*/
	virtual CPoint GetCenter() const = 0;

	/**
		Returns the bounding box of this 2D-object.
	*/
	virtual CRectangle GetBoundingBox() const = 0;
};


} // namespace i2d


#endif // !i2d_IObject2d_included

