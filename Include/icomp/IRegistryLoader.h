// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QString>


// ACF includes
#include <istd/IPolymorphic.h>


namespace icomp
{


class IRegistry;


/**
	Interface for a ACF registry loader.
*/
class IRegistryLoader: virtual public istd::IPolymorphic
{
public:
	/**
		Get an ACF registry from a given file.
	*/
	virtual const icomp::IRegistry* GetRegistryFromFile(const QString& path) const = 0;
};
	

} // namespace icomp


