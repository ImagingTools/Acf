#include "icam/CExposureParamsComp.h"


#include "istd/TChangeNotifier.h"

#include "iser/CArchiveTag.h"


namespace icam
{


CExposureParamsComp::CExposureParamsComp()
:	m_shutterTime(0.001), m_delayTime(0), m_eenDelayTime(0)
{
}


// reimplemented (icam::IExposureParams)

double CExposureParamsComp::GetShutterTime() const
{
	if (*m_enableShutterTimeAttrPtr){
		return m_shutterTime;
	}
	else{
		return -1;
	}
}


bool CExposureParamsComp::SetShutterTime(double time)
{
	istd::CRange range = GetShutterTimeRange();
	if (range.IsInside(time)){
		istd::CChangeNotifier notifier(this);

		m_shutterTime = time;

		return true;
	}
	
	return false;
}


double CExposureParamsComp::GetDelayTime() const
{
	if (*m_enableDelayTimeAttrPtr){
		return m_delayTime;
	}
	else{
		return -1;
	}
}


bool CExposureParamsComp::SetDelayTime(double time)
{
	istd::CRange range = GetDelayTimeRange();
	if (range.IsInside(time)){
		istd::CChangeNotifier notifier(this);

		m_delayTime = time;

		return true;
	}
	
	return false;
}


double CExposureParamsComp::GetEenDelay() const
{
	if (*m_enableEenDelayTimeAttrPtr){
		return m_eenDelayTime;
	}
	else{
		return -1;
	}
}


bool CExposureParamsComp::SetEenDelayRange(double time)
{
	istd::CRange range = GetEenDelayRange();
	if (range.IsInside(time)){
		istd::CChangeNotifier notifier(this);

		m_eenDelayTime = time;

		return true;
	}
	
	return false;
}


// reimplemented (icam::IExposureInfo)

istd::CRange CExposureParamsComp::GetShutterTimeRange() const
{
	if (m_exposureInfoCompPtr.IsValid() && *m_enableShutterTimeAttrPtr){
		return m_exposureInfoCompPtr->GetShutterTimeRange();
	}
	else{
		return istd::CRange(-1, -1);
	}
}


istd::CRange CExposureParamsComp::GetDelayTimeRange() const
{
	if (m_exposureInfoCompPtr.IsValid() && *m_enableDelayTimeAttrPtr){
		return m_exposureInfoCompPtr->GetDelayTimeRange();
	}
	else{
		return istd::CRange(-1, -1);
	}
}


istd::CRange CExposureParamsComp::GetEenDelayRange() const
{
	if (m_exposureInfoCompPtr.IsValid() && *m_enableEenDelayTimeAttrPtr){
		return m_exposureInfoCompPtr->GetEenDelayRange();
	}
	else{
		return istd::CRange(-1, -1);
	}
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


