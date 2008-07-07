#ifndef icam_IExposureInfo_included
#define icam_IExposureInfo_included


#include "istd/IPolymorphic.h"
#include "istd/CRange.h"


namespace icam
{


class IExposureInfo: virtual public istd::IPolymorphic
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


#endif // !icam_IExposureInfo_included


