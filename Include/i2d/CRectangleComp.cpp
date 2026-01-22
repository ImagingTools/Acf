// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <i2d/CRectangleComp.h>


namespace i2d
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CRectangleComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_defaultObjectProviderCompPtr.IsValid()){
		return;
	}

	Q_ASSERT(m_leftAttrPtr.IsValid());
	Q_ASSERT(m_topAttrPtr.IsValid());
	Q_ASSERT(m_widthAttrPtr.IsValid());
	Q_ASSERT(m_heightAttrPtr.IsValid());

	SetLeft(*m_leftAttrPtr);
	SetTop(*m_topAttrPtr);
	SetRight(*m_leftAttrPtr + *m_widthAttrPtr);
	SetBottom(*m_topAttrPtr + *m_heightAttrPtr);
}


} // namespace i2d


