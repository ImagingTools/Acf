// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <icomp/CComponentBase.h>
#include <iprm/CEnableableParam.h>


namespace iprm
{


class CEnableableParamComp:
			public icomp::CComponentBase,
			public iprm::CEnableableParam
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CEnableableParamComp);
		I_REGISTER_INTERFACE(iprm::IEnableableParam);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_ASSIGN(m_defaultIsEnabledAttrPtr, "DefaultEnabled", "Default value", true, false);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

private:
	I_ATTR(bool, m_defaultIsEnabledAttrPtr);
};


} // namespace iprm




