#ifndef icam_IExposureParams_included
#define icam_IExposureParams_included


#include "iser/ISerializable.h"

#include "icam/IExposureInfo.h"


namespace icam
{


class IExposureParams: virtual public IExposureInfo, virtual public iser::ISerializable
{
public:
	/**
		Get shutter time (exposure time).
		\return	shutter time in seconds or negative value if this value isn't active.
	*/
	virtual double GetShutterTime() const = 0;

	/**
		Set shutter time (exposure time).
		\param	time	shutter time in seconds.
		\return	true, if success.
	*/
	virtual bool SetShutterTime(double time) = 0;

	/**
		Get delay time.
		Delay time is time between trigger and exposure begin.
		\return	delay time in seconds or negative value if this value isn't active.
	*/
	virtual double GetDelayTime() const = 0;

	/**
		Set delay time.
		Delay time is time between trigger and exposure begin.
		\param	time	delay time in seconds.
		\return	true, if success.
	*/
	virtual bool SetDelayTime(double time) = 0;

	/**
		Get exposure enabled (EEN) signal delay time.
		\return	delay time in seconds or negative value if this value isn't active.
	*/
	virtual double GetEenDelay() const = 0;

	/**
		Set exposure enabled (EEN) signal delay time.
		\param	time	exposure enabled (EEN) signal delay time in seconds.
		\return	true, if success.
	*/
	virtual bool SetEenDelay(double time) = 0;
};


} // namespace icam


#endif // !icam_IExposureParams_included


