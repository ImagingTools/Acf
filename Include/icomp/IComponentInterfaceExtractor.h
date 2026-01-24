// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/IPolymorphic.h>
#include <istd/CClassInfo.h>


namespace icomp
{


class IComponent;


/**
	Provide extracting of some interface from component.
*/
class IComponentInterfaceExtractor: virtual public istd::IPolymorphic
{
public:
	/**
		Extract instance implemented specified interface from some component.
	*/
	virtual void* GetComponentInterface(
				const istd::CClassInfo& interfaceType,
				IComponent& component,
				const QByteArray& subId) const = 0;
};


} // namespace icomp




