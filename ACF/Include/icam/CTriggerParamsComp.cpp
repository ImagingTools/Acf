#include "icam/CTriggerParamsComp.h"


#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace icam
{


CTriggerParamsComp::CTriggerParamsComp()
	:m_isTriggerEnabled(false),
	m_triggerMode(TM_RISING_EDGE)
{
}


// reimplemented (icam::ITriggerParams)

const ITriggerConstraints* CTriggerParamsComp::GetConstraints() const
{
	return m_triggerConstraintsCompPtr.GetPtr();
}


bool CTriggerParamsComp::IsTriggerEnabled() const
{
	return m_isTriggerEnabled;
}


void CTriggerParamsComp::SetTriggerEnabled(bool isEnabled)
{
	if (m_isTriggerEnabled != isEnabled){
		istd::CChangeNotifier changePtr(this);

		m_isTriggerEnabled = isEnabled;
	}
}


int CTriggerParamsComp::GetTriggerMode() const
{
	return m_triggerMode;
}


void CTriggerParamsComp::SetTriggerMode(int triggerMode)
{
	if (m_triggerMode!= triggerMode){
		istd::CChangeNotifier changePtr(this);

		m_triggerMode = triggerMode;
	}
}


// reimplemented (iser::ISerializable)

bool CTriggerParamsComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag isTriggerEnabledTag("TriggerEnabled", "Is external trigger enabled");

	bool retVal = archive.BeginTag(isTriggerEnabledTag);
	retVal = retVal && archive.Process(m_isTriggerEnabled);
	retVal = retVal && archive.EndTag(isTriggerEnabledTag);

	static iser::CArchiveTag bottomTag("TriggerMode", "Trigger mode");
	retVal = retVal && archive.BeginTag(bottomTag);
	retVal = retVal && archive.Process(m_triggerMode);
	retVal = retVal && archive.EndTag(bottomTag);

	return retVal;
}


// reimplemented (icomp::IComponent)

void CTriggerParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_isTriggerEnabledAttrPtr.IsValid()){
		m_isTriggerEnabled = *m_isTriggerEnabledAttrPtr;
	}

	if (m_triggerModeAttrPtr.IsValid()){
		m_triggerMode = *m_triggerModeAttrPtr;
	}
}


} // namespace icam


