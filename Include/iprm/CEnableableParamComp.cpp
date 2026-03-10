// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iprm/CEnableableParamComp.h>


namespace iprm
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CEnableableParamComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_defaultIsEnabledAttrPtr.IsValid()){
		SetEnabled(*m_defaultIsEnabledAttrPtr);
	}
}


} // namespace iprm


