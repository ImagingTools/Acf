// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <i2d/CPosition2dComp.h>


namespace i2d
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CPosition2dComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_defaultObjectProviderCompPtr.IsValid()){
		return;
	}

	Q_ASSERT(m_xAttrPtr.IsValid());
	Q_ASSERT(m_yAttrPtr.IsValid());

	SetPosition(i2d::CVector2d(*m_xAttrPtr, *m_yAttrPtr));
}


} // namespace i2d


