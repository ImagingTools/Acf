#pragma once


// ACF includes
#include <i2d/CRectangle.h>


namespace i2d
{


/**
	Interface for defining constraints on rectangle dimensions.
	Provides methods to retrieve valid ranges for width and height.
*/
class IRectangleConstraints: virtual public istd::IPolymorphic
{
public:
	/**
		Get the valid range for rectangle width.
		\return	Range of acceptable width values.
	*/
	virtual istd::CIntRange GetWidthRange() const = 0;
	/**
		Get the valid range for rectangle height.
		\return	Range of acceptable height values.
	*/
	virtual istd::CIntRange GetHeightRange() const = 0;
};


} // namespace i2d


