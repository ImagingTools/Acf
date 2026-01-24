// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iprm/CIdParamComp.h>


namespace iprm
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CIdParamComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_defaultIdAttrPtr.IsValid()){
		BaseClass2::SetId(*m_defaultIdAttrPtr);
	}
}


} // namespace iprm


