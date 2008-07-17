#include "icam/CExposureParamsComp.h"


#include "istd/TChangeNotifier.h"
#include "istd/CRange.h"

#include "iser/CArchiveTag.h"


namespace icam
{


CExposureParamsComp::CExposureParamsComp()
:	m_shutterTime(0.001), m_delayTime(0), m_eenDelayTime(0)
{
}


// reimplemented (icomp::IComponent)

void CExposureParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	I_ASSERT(m_defaultShutterTimeAttrPtr.IsValid());	// isObligatory is set
	m_shutterTime = *m_defaultShutterTimeAttrPtr;
}


// reimplemented (icam::IExposureParams)

const IExposureConstraints* CExposureParamsComp::GetConstraints() const
{
	return m_exposureConstraintsCompPtr.GetPtr();
}


double CExposureParamsComp::GetShutterTime() const
{
	return m_shutterTime;
}


bool CExposureParamsComp::SetShutterTime(double time)
{
	if (m_exposureConstraintsCompPtr.IsValid()){
		istd::CRange range = m_exposureConstraintsCompPtr->GetShutterTimeRange();

		if (!range.Contains(time)){
			return false;
		}
	}

	if (time != m_shutterTime){
		istd::CChangeNotifier notifier(this);

		m_shutterTime = time;
	}

	return true;
}


double CExposureParamsComp::GetDelayTime() const
{
	return m_delayTime;
}


bool CExposureParamsComp::SetDelayTime(double time)
{
	if (m_exposureConstraintsCompPtr.IsValid()){
		istd::CRange range = m_exposureConstraintsCompPtr->GetDelayTimeRange();

		if (!range.Contains(time)){
			return false;
		}
	}

	if (time != m_delayTime){
		istd::CChangeNotifier notifier(this);

		m_delayTime = time;
	}
	
	return true;
}


double CExposureParamsComp::GetEenDelay() const
{
	return m_eenDelayTime;
}


bool CExposureParamsComp::SetEenDelay(double time)
{
	if (m_exposureConstraintsCompPtr.IsValid()){
		istd::CRange range = m_exposureConstraintsCompPtr->GetEenDelayRange();
		if (!range.Contains(time)){
			return false;
		}
	}

	if (time != m_eenDelayTime){
		istd::CChangeNotifier notifier(this);

		m_eenDelayTime = time;
	}

	return true;
}


// reimplemented (iser::ISerializable)

bool CExposureParamsComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag shutterTimeTag("ShutterTime", "Shutter time in seconds");
	retVal = retVal && archive.BeginTag(shutterTimeTag);
	retVal = retVal && archive.Process(m_shutterTime);
	retVal = retVal && archive.EndTag(shutterTimeTag);

	static iser::CArchiveTag delayTimeTag("DelayTime", "Delay of shutter in seconds");
	retVal = retVal && archive.BeginTag(delayTimeTag);
	retVal = retVal && archive.Process(m_delayTime);
	retVal = retVal && archive.EndTag(delayTimeTag);

	static iser::CArchiveTag eenDelayTimeTag("EenDelayTime", "Delay of exposure enabled signal in seconds");
	retVal = retVal && archive.BeginTag(eenDelayTimeTag);
	retVal = retVal && archive.Process(m_eenDelayTime);
	retVal = retVal && archive.EndTag(eenDelayTimeTag);

	return retVal;
}


} // namespace icam


