#include "isec/CFixedAuthorisationVerifierComp.h"


namespace isec
{


// reimplemented (icomp::IComponent)

void CFixedAuthorisationVerifierComp::OnComponentCreated()
{
	m_enabledFeatureIds.clear();

	int enabledCount = m_enabledFeaturesCompPtr.GetCount();
	for (int enabledIndex = 0; enabledIndex < enabledCount; ++enabledIndex){
		const istd::CString& id = m_enabledFeaturesCompPtr[enabledIndex];

		m_enabledFeatureIds.insert(id.ToString());
	}

	m_disabledFeatureIds.clear();

	int disabledCount = m_disabledFeaturesCompPtr.GetCount();
	for (int disabledIndex = 0; disabledIndex < disabledCount; ++disabledIndex){
		const istd::CString& id = m_disabledFeaturesCompPtr[disabledIndex];

		m_disabledFeatureIds.insert(id.ToString());
	}
}


// reimplemented (isec::IAuthorizationVerifier)

bool CFixedAuthorisationVerifierComp::IsAuthorized(const std::string& featureId) const
{
	if (m_enabledFeatureIds.find(featureId) != m_enabledFeatureIds.end()){
		return true;
	}

	if (m_disabledFeatureIds.find(featureId) != m_disabledFeatureIds.end()){
		return false;
	}

	if (m_slaveVerifierCompPtr.IsValid()){
		return m_slaveVerifierCompPtr->IsAuthorized(featureId);
	}

	if (m_defaultRightCompPtr.IsValid()){
		return *m_defaultRightCompPtr;
	}

	return false;
}


isec::IAuthorizationVerifier::Ids CFixedAuthorisationVerifierComp::GetKnownFeatureIds() const
{
	Ids retVal = m_enabledFeatureIds;
	retVal.insert(m_disabledFeatureIds.begin(), m_disabledFeatureIds.end());

	if (m_slaveVerifierCompPtr.IsValid()){
		Ids slaveIds = m_slaveVerifierCompPtr->GetKnownFeatureIds();

		retVal.insert(slaveIds.begin(), slaveIds.end());
	}

	return retVal;
}


} // namespace isec


