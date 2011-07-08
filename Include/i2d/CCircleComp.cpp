#include "i2d/CCircleComp.h"


namespace i2d
{		

// protected methods

// reimplemented (icomp::CComponentBase)

void CCircleComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	I_ASSERT(m_centerXAttrPtr.IsValid());
	I_ASSERT(m_centerYAttrPtr.IsValid());
	I_ASSERT(m_radiusAttrPtr.IsValid());

	SetPosition(i2d::CVector2d(*m_centerXAttrPtr, *m_centerXAttrPtr));
	SetRadius(*m_radiusAttrPtr);
}


} // namespace i2d


