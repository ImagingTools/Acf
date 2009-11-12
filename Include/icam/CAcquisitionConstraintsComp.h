#ifndef icam_CAcquisitionConstraintsComp_included
#define icam_CAcquisitionConstraintsComp_included


#include "icomp/CComponentBase.h"

#include "icam/ITriggerConstraints.h"
#include "icam/IExposureConstraints.h"


namespace icam
{


/**
	Component for the static configuration of the acquisition constraints for a given camera device.
*/
class CAcquisitionConstraintsComp:
			public icomp::CComponentBase,
			virtual public ITriggerConstraints,
			virtual public IExposureConstraints		
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CAcquisitionConstraintsComp);
		I_REGISTER_INTERFACE(ITriggerConstraints);
		I_REGISTER_INTERFACE(IExposureConstraints);
		I_ASSIGN(m_isTriggerSupportedAttrPtr, "TriggerSupported", "Trigger mode is supported by the device", true, false);
		I_ASSIGN(m_isRisingEdgeTriggerSupportedAttrPtr, "RisingEdgeTriggerSupported", "Rising edge trigger is supported by the device", true, false);
		I_ASSIGN(m_isFallingEdgeTriggerSupportedAttrPtr, "FallingEdgeTriggerSupported", "Falling edge trigger is supported by the device", true, false);
		I_ASSIGN(m_isNegativeLevelTriggerSupportedAttrPtr, "NegativeLevelTriggerSupported", "Negative level trigger is supported by the device", true, false);
		I_ASSIGN(m_isPositiveLevelTriggerSupportedAttrPtr, "PositiveLevelTriggerSupported", "Positive level trigger is supported by the device", true, false);
		I_ASSIGN(m_minShutterTimeAttrPtr, "MinShutterTime", "Minimal shutter time for this device", true, 0.0);
		I_ASSIGN(m_maxShutterTimeAttrPtr, "MaxShutterTime", "Maximal shutter time for this device", true, 0.0);
		I_ASSIGN(m_minDelayTimeAttrPtr, "MinDelayTime", "Minimal delay time for this device", true, 0.0);
		I_ASSIGN(m_maxDelayTimeAttrPtr, "MaxDelayTime", "Maximal delay time for this device", true, 0.0);
		I_ASSIGN(m_minEenDelayAttrPtr, "MinEenDelay", "Minimal EEN-delay for this device", true, 0.0);
		I_ASSIGN(m_maxEenDelayAttrPtr, "MaxEenDelay", "Maximal EEN-delay for this device", true, 0.0);
	I_END_COMPONENT;

	// reimplemented (icam::IExposureConstraints)
	virtual bool IsTriggerSupported() const;
	virtual bool IsTriggerModeSupported(int triggerMode) const;

	// reimplemented (icam::IExposureConstraints)
	virtual istd::CRange GetShutterTimeRange() const;
	virtual istd::CRange GetDelayTimeRange() const;
	virtual istd::CRange GetEenDelayRange() const;

private:
	I_ATTR(bool, m_isTriggerSupportedAttrPtr);
	I_ATTR(bool, m_isRisingEdgeTriggerSupportedAttrPtr);
	I_ATTR(bool, m_isFallingEdgeTriggerSupportedAttrPtr);
	I_ATTR(bool, m_isPositiveLevelTriggerSupportedAttrPtr);
	I_ATTR(bool, m_isNegativeLevelTriggerSupportedAttrPtr);
	I_ATTR(double, m_minShutterTimeAttrPtr);
	I_ATTR(double, m_maxShutterTimeAttrPtr);
	I_ATTR(double, m_minDelayTimeAttrPtr);
	I_ATTR(double, m_maxDelayTimeAttrPtr);
	I_ATTR(double, m_minEenDelayAttrPtr);
	I_ATTR(double, m_maxEenDelayAttrPtr);
};


} // namespace icam


#endif // !icam_CAcquisitionConstraintsComp_included


