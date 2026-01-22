// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <i2d/CArcComp.h>


namespace i2d
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CArcComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_defaultObjectProviderCompPtr.IsValid()){
		return;
	}

	Q_ASSERT(m_centerXAttrPtr.IsValid());
	Q_ASSERT(m_centerYAttrPtr.IsValid());
	Q_ASSERT(m_radiusAttrPtr.IsValid());
	Q_ASSERT(m_startAngleAttrPtr.IsValid());
	Q_ASSERT(m_endAngleAttrPtr.IsValid());

	SetPosition(i2d::CVector2d(*m_centerXAttrPtr, *m_centerXAttrPtr));
	SetRadius(*m_radiusAttrPtr);
	SetStartAngle(*m_startAngleAttrPtr);
	SetEndAngle(*m_endAngleAttrPtr);
}

} // namespace i2d


