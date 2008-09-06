#ifndef icam_ITriggerParams_included
#define icam_ITriggerParams_included


#include "iser/ISerializable.h"

#include "icam/icam.h"


namespace icam
{


class ITriggerConstraints;


/**
	Interface for AVT camera parameters.
*/
class ITriggerParams: virtual public iser::ISerializable
{
public:
	/**
		Describe working mode of trigger signal.
	*/
	enum TriggerMode
	{
		TM_DEFAULT,
		TM_RISING_EDGE,
		TM_FALLING_EDGE,
		TM_POSITIVE_LEVEL,
		TM_NEGATIVE_LEVEL
	};

	/**
		Get constrints describing supported parameter states.
	*/
	virtual const ITriggerConstraints* GetConstraints() const = 0;

	/**
		Check if trigger is enabled.
	*/
	virtual bool IsTriggerEnabled() const = 0;
	/**
		Set trigger enabling state.
	*/
	virtual void SetTriggerEnabled(bool isEnabled) = 0;

	/**
		Get trigger mode.
		Normal trigger modes are defined in \c icam::ITriggerParams::TriggerMode.
	*/
	virtual int GetTriggerMode() const = 0;

	/**
		Set trigger mode.
		Normal trigger modes are defined in \c icam::ITriggerParams::TriggerMode.
	*/
	virtual void SetTriggerMode(int triggerMode) = 0;
};


} // namespace icam


#endif // !icam_ITriggerParams_included


