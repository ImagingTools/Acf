#ifndef isec_CFixedAuthorisationVerifierComp_included
#define isec_CFixedAuthorisationVerifierComp_included


#include "icomp/CComponentBase.h"

#include "isec/IAuthorizationVerifier.h"


namespace isec
{


class CFixedAuthorisationVerifierComp:
			public icomp::CComponentBase,
			virtual public IAuthorizationVerifier
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CFixedAuthorisationVerifierComp);
		I_REGISTER_INTERFACE(IAuthorizationVerifier);
		I_ASSIGN_MULTI_0(m_enabledFeaturesCompPtr, "EnabledIds", "List of feature ID's will be enabled", false);
		I_ASSIGN_MULTI_0(m_disabledFeaturesCompPtr, "DisabledIds", "List of feature ID's will be disabled", false);
		I_ASSIGN(m_slaveVerifierCompPtr, "SlaveVerifier", "Slave verifier used to obtain default value", false, "SlaveVerifier");
		I_ASSIGN(m_defaultRightCompPtr, "DefaultRight", "Default right will be returned if ID is unknown", false, false);
	I_END_COMPONENT;

	// reimplemented (isec::IAuthorizationVerifier)
	virtual bool IsAuthorized(const std::string& featureId) const;
	virtual Ids GetKnownFeatureIds() const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_MULTIATTR(istd::CString, m_enabledFeaturesCompPtr);
	I_MULTIATTR(istd::CString, m_disabledFeaturesCompPtr);
	I_REF(IAuthorizationVerifier, m_slaveVerifierCompPtr);
	I_ATTR(bool, m_defaultRightCompPtr);

	typedef std::set<std::string> FeatureIds;

	Ids m_enabledFeatureIds;
	Ids m_disabledFeatureIds;
};


} // namespace isec


#endif // !isec_CFixedAuthorisationVerifierComp_included


