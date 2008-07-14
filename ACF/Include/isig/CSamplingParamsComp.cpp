#include "isig/CSamplingParamsComp.h"


#include "istd/TChangeNotifier.h"

#include "isig/ISamplingConstraints.h"


namespace isig
{


CSamplingParamsComp::CSamplingParamsComp()
:	m_interval(0), m_samplingMode(SM_SINGLE)
{
}


// reimplemented (icomp::IComponent)

void CSamplingParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_samplingConstraintsCompPtr.IsValid()){
		for (int i = 0; i <= SM_LAST; ++i){
			if (m_samplingConstraintsCompPtr->IsSamplingModeSupported(i)){
				m_samplingMode = i;
				break;
			}
		}
		istd::CRange intervalRange = m_samplingConstraintsCompPtr->GetIntervalRange();
		m_interval = sqrt(intervalRange.GetMinValue() * intervalRange.GetMaxValue());
	}
}


// reimplemented (isig::ISamplingParams)

const ISamplingConstraints* CSamplingParamsComp::GetConstraints() const
{
	return m_samplingConstraintsCompPtr.GetPtr();
}


double CSamplingParamsComp::GetInterval() const
{
	return m_interval;
}


void CSamplingParamsComp::SetInterval(double value)
{
	if (value != m_interval){
		istd::CChangeNotifier notifier(this);

		m_interval = value;
	}
}


int CSamplingParamsComp::GetSamplingMode() const
{
	return m_samplingMode;
}


bool CSamplingParamsComp::SetSamplingMode(int mode)
{
	if (mode != m_samplingMode){
		if (m_samplingConstraintsCompPtr.IsValid()){
			if (!m_samplingConstraintsCompPtr->IsSamplingModeSupported(mode)){
				return false;
			}
		}

		istd::CChangeNotifier notifier(this);

		m_samplingMode = mode;
	}

	return true;
}


// reimplemented (iser::ISerializable)

bool CSamplingParamsComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	double interval = m_interval;
	static iser::CArchiveTag intervalTag("Interval", "Interval [s] of simple sample");
	retVal = retVal && archive.BeginTag(intervalTag);
	retVal = retVal && archive.Process(interval);
	retVal = retVal && archive.EndTag(intervalTag);

	int samplingMode = m_samplingMode;
	static iser::CArchiveTag samplingModeTag("SamplingMode", "Sampling mode (isig::ISamplingParams::SamplingMode)");
	retVal = retVal && archive.BeginTag(samplingModeTag);
	retVal = retVal && archive.Process(samplingMode);
	retVal = retVal && archive.EndTag(samplingModeTag);

	if (!archive.IsStoring()){
		istd::CChangeNotifier notifier(this);

		SetInterval(interval);
		SetSamplingMode(samplingMode);
	}

	return retVal;
}


} // namespace isig


