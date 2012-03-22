#ifndef i2d_ICalibrationProvider_included
#define i2d_ICalibrationProvider_included


// ACF includes
#include "istd/IChangeable.h"

#include "i2d/ITransformation2d.h"


namespace i2d
{


/**
	Common interface for a calibration provider.
*/
class ICalibrationProvider: virtual public istd::IChangeable
{
public:
	/**
		Get access to transformation object, which transforms a coordinate system to a logical one.
		\return	pointer to transformation if it is accessible, or NULL.
	*/
	virtual const i2d::ITransformation2d* GetLogicalTransform() const = 0;
};


} // namespace i2d


#endif // !i2d_ICalibrationProvider_included


