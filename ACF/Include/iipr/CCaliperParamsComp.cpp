#include "iipr/CCaliperParamsComp.h"


namespace iipr
{


// reimplemented (icomp::IComponent)

void CCaliperParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	SetWeightThreshold(*m_weightThresholdAttrPtr);
	SetPolarityMode(*m_polarityModeAttrPtr);
	SetDirectionMode(*m_directionModeAttrPtr);
}


} // namespace iipr


