// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/IChangeable.h>
#include <ibase/ICommand.h>


namespace ibase
{

	
/**
	Interface for a provider of the heriarchical commands.
*/
class ICommandsProvider: virtual public istd::IChangeable
{
public:
	enum ChangeFlags
	{
		CF_COMMANDS = 0x62b7e8
	};

	/**
		Get list of menu commands.
		These commands will be integrated in global menu system, independent from actual selected view.
		For support of normal pull down menu, depth of this tree structure should be at least 3.
	*/
	virtual const IHierarchicalCommand* GetCommands() const;
};


inline const IHierarchicalCommand* ICommandsProvider::GetCommands() const
{
	return NULL;
}


} // namespace ibase




