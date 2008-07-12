#ifndef icam_CExposureParamsComp_included
#define icam_CExposureParamsComp_included


#include "icomp/CComponentBase.h"

#include "icam/IExposureParams.h"


namespace icam
{


class CExposureParamsComp: public icomp::CComponentBase, virtual public IExposureParams
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CExposureParamsComp);
		I_REGISTER_INTERFACE(IExposureParams);
		I_REGISTER_INTERFACE(IExposureInfo);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_exposureInfoCompPtr, "ExposureInfo", "Information about valid exposure parameters", true, "ExposureInfo");
		I_ASSIGN(m_enableShutterTimeAttrPtr, "EnableShutterTime", "Enables shutter time", true, true);
		I_ASSIGN(m_enableDelayTimeAttrPtr, "EnableDelayTime", "Enables delay time", true, true);
		I_ASSIGN(m_enableEenDelayTimeAttrPtr, "EnableEenDelayTime", "Enables EEN (exposure enabled) delay time", true, true);
	I_END_COMPONENT;

	CExposureParamsComp();

	// reimplemented (icam::IExposureParams)
	virtual double GetShutterTime() const;
	virtual bool SetShutterTime(double time);
	virtual double GetDelayTime() const;
	virtual bool SetDelayTime(double time);
	virtual double GetEenDelay() const;
	virtual bool SetEenDelay(double time);

	// reimplemented (icam::IExposureInfo)
	virtual istd::CRange GetShutterTimeRange() const;
	virtual istd::CRange GetDelayTimeRange() const;
	virtual istd::CRange GetEenDelayRange() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	I_REF(IExposureInfo, m_exposureInfoCompPtr);
	I_ATTR(bool, m_enableShutterTimeAttrPtr);
	I_ATTR(bool, m_enableDelayTimeAttrPtr);
	I_ATTR(bool, m_enableEenDelayTimeAttrPtr);

	double m_shutterTime;
	double m_delayTime;
	double m_eenDelayTime;
};


} // namespace icam


#endif // !icam_CExposureParamsComp_included


