// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ibase/CInstantiatorComp.h>


namespace ibase
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CInstantiatorComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	int elementsCount = m_instancesAttrPtr.GetCount();
	for (int i = 0; i < elementsCount; ++i){
		m_instancesAttrPtr[i];
	}
}


} // namespace ibase


