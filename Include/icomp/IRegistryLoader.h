// SPDX-License-Identifier: LGPL-2.1-only
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


