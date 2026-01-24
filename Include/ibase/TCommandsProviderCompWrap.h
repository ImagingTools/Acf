// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <imod/TModelWrap.h>
#include <icomp/CComponentBase.h>
#include <ibase/ICommandsProvider.h>


namespace ibase
{


/**
	Generic macro for ICommandsProvider implementation.
*/
template <class Base>
class TCommandsProviderCompWrap:
			public Base,
			public imod::TModelWrap<ibase::ICommandsProvider>
{
public:
	typedef Base BaseClass;

	I_BEGIN_COMPONENT(TCommandsProviderCompWrap);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
	I_END_COMPONENT;
};


} // namespace ibase




