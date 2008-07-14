#ifndef icam_IExposureConstraints_included
#define icam_IExposureConstraints_included


#include "istd/IPolymorphic.h"
#include "istd/CRange.h"

#include "icam/icam.h"


namespace icam
{


class IExposureConstraints: virtual public istd::IPolymorphic
{
public:
	/**
		Get range of posible shutter time (exposure time) values.
		\return	range of shutter time, or empty range is this time cannot be controlled.
	*/
	virtual istd::CRange GetShutterTimeRange() const = 0;

	/**
		Get range of posible delay time values.
		Delay time is time between trigger and exposure begin.
		\return	range of delay time, or empty range is this time cannot be controlled.
	*/
	virtual istd::CRange GetDelayTimeRange() const = 0;

	/**
		Get range of posible exposure enabled (EEN) signal delay time values.
		\return	range of delay time, or empty range is this time cannot be controlled.
	*/
	virtual istd::CRange GetEenDelayRange() const = 0;
};


} // namespace icam


#endif // !icam_IExposureConstraints_included


