// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/IPolymorphic.h>
#include <ibase/ICommandsProvider.h>


namespace ibase
{

	
/**
	Interface for a displayer of the hierarchical commands.
*/
class ICommandsDisplayer : virtual public istd::IPolymorphic
{
public:
	virtual void ShowCommands(const ibase::ICommandsProvider* commandsProvider) = 0;
};


} // namespace ibase




