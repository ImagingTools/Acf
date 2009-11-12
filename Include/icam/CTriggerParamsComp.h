#ifndef icam_CTriggerParamsComp_included
#define icam_CTriggerParamsComp_included


#include "icomp/CComponentBase.h"

#include "icam/ITriggerConstraints.h"
#include "icam/ITriggerParams.h"


namespace icam
{


class CTriggerParamsComp: public icomp::CComponentBase, virtual public icam::ITriggerParams
{
public:
	typedef icomp::CComponentBase BaseClass;

	CTriggerParamsComp();

	I_BEGIN_COMPONENT(CTriggerParamsComp);
		I_REGISTER_INTERFACE(icam::ITriggerParams);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_triggerConstraintsCompPtr, "TriggerConstraints", "Describing allowed parameter state, tipically implemented in camera component", false, "TriggerConstraints");
		I_ASSIGN(m_isTriggerEnabledAttrPtr, "ExternalTriggerEnabled", "Enables external trigger", false, false);
		I_ASSIGN(m_triggerModeAttrPtr, "TriggerMode", "Set trigger mode: 0 - Default, 1 - Rising edge, 2 - Falling edge, 3 - Positive level, 4 - Negative level", false, 0);
	I_END_COMPONENT;

	// reimplmented (icam::ITriggerParams)
	virtual const ITriggerConstraints* GetConstraints() const;
	virtual bool IsTriggerEnabled() const;
	virtual void SetTriggerEnabled(bool isEnabled);
	virtual int GetTriggerMode() const;
	virtual void SetTriggerMode(int triggerMode);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

private:
	bool m_isTriggerEnabled;
	int m_triggerMode;

	I_REF(icam::ITriggerConstraints, m_triggerConstraintsCompPtr);
	I_ATTR(bool, m_isTriggerEnabledAttrPtr);
	I_ATTR(int, m_triggerModeAttrPtr);
};


} // namespace icam


#endif // !icam_CTriggerParamsComp_included


