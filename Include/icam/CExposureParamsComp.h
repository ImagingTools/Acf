#ifndef icam_CExposureParamsComp_included
#define icam_CExposureParamsComp_included


#include "icomp/CComponentBase.h"

#include "icam/IExposureParams.h"
#include "icam/IExposureConstraints.h"


namespace icam
{


class CExposureParamsComp: public icomp::CComponentBase, virtual public IExposureParams
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CExposureParamsComp);
		I_REGISTER_INTERFACE(IExposureParams);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_exposureConstraintsCompPtr, "ExposureConstraints", "Information about valid exposure parameters", false, "ExposureConstraints");
		I_ASSIGN(m_defaultShutterTimeAttrPtr, "DefaultShutterTime", "Default value of shutter time in seconds", true, 0.001);
	I_END_COMPONENT;

	CExposureParamsComp();

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

	// reimplemented (icam::IExposureParams)
	virtual const IExposureConstraints* GetConstraints() const;
	virtual double GetShutterTime() const;
	virtual bool SetShutterTime(double time);
	virtual double GetDelayTime() const;
	virtual bool SetDelayTime(double time);
	virtual double GetEenDelay() const;
	virtual bool SetEenDelay(double time);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	I_REF(IExposureConstraints, m_exposureConstraintsCompPtr);
	I_ATTR(double, m_defaultShutterTimeAttrPtr);

	double m_shutterTime;
	double m_delayTime;
	double m_eenDelayTime;
};


} // namespace icam


#endif // !icam_CExposureParamsComp_included


