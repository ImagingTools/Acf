// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <icomp/CComponentBase.h>
#include <iprm/CNameParam.h>


namespace ibase
{


/**
	Component for automatic creation of the Universally Unique Identifier (UUID).
	You can access this UUID using \c GetName method defined in interface \c iprm::INameParam.
*/
class CUuidComp:
			public icomp::CComponentBase,
			public iprm::CNameParam
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CUuidComp);
		I_REGISTER_INTERFACE(iprm::INameParam);
		I_REGISTER_INTERFACE(iser::ISerializable);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
};


} // namespace ibase




