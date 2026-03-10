// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <i2d/CLine2dComp.h>


namespace i2d
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CLine2dComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_defaultObjectProviderCompPtr.IsValid()){
		return;
	}

	Q_ASSERT(m_x1AttrPtr.IsValid());
	Q_ASSERT(m_y1AttrPtr.IsValid());
	Q_ASSERT(m_x2AttrPtr.IsValid());
	Q_ASSERT(m_y2AttrPtr.IsValid());

	SetPoint1(i2d::CVector2d(*m_x1AttrPtr, *m_y1AttrPtr));
	SetPoint2(i2d::CVector2d(*m_x2AttrPtr, *m_y2AttrPtr));
}


} // namespace i2d


