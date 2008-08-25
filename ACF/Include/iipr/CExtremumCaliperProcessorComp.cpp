#include "iipr/CExtremumCaliperProcessorComp.h"


namespace iipr
{


// reimplemented (icomp::IComponent)

void CExtremumCaliperProcessorComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	I_ASSERT(m_caliperParamsIdAttrPtr.IsValid());	// isObligatory was set 'true'
	SetCaliperParamsId((*m_caliperParamsIdAttrPtr).ToString());
}


} // namespace iipr

