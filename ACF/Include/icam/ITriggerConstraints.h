#ifndef icam_ITriggerConstraints_included
#define icam_ITriggerConstraints_included


#include "istd/IPolymorphic.h"


namespace icam
{


/**
	Interface for AVT camera parameters.
*/
class ITriggerConstraints: virtual public istd::IPolymorphic
{
public:
	/**
		Check if trigger enabling/disabling is supported.
		Please note, that for implementations using only triggered acuisition it should return also false.
	*/
	virtual bool IsTriggerSupported() const = 0;

	/**
		Check if specified trigger mode is supported.
		Default mode should be always supported.
		Normal trigger modes are defined in \c icam::ITriggerParams::TriggerMode.
	*/
	virtual bool IsTriggerModeSupported(int triggerMode) const = 0;
};


} // namespace icam


#endif // !icam_ITriggerConstraints_included


