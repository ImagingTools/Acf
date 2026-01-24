// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <icomp/IComponentListProvider.h>


namespace icomp
{


class IRegistry;


/**
	Allow access to registries defined by composed components.
*/
class IRegistriesManager: virtual public IComponentListProvider
{
public:
	/**
		Get access to stored registry.
		\param	address		adress of registry treated as composed component.
		\return				registry object or NULL if specified composite component is not registered.
	*/
	virtual const IRegistry* GetRegistry(const CComponentAddress& address, const IRegistry* contextRegistryPtr = NULL) const = 0;
};


} // namespace icomp


