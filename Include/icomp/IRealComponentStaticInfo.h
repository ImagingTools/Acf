// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <icomp/IComponentStaticInfo.h>
#include <icomp/IComponent.h>
#include <icomp/IComponentInterfaceExtractor.h>


namespace icomp
{


class IComponent;


/**
	Interface adding to component static info functionality existing only for real components.
	Real components are components which can be created in memory.
*/
class IRealComponentStaticInfo:
			virtual public IComponentStaticInfo,
			virtual public IComponentInterfaceExtractor
{
public:
	/**
		Create component instance.
		\return				pointer to created component or NULL if this component cannot be created.
	 */
	virtual icomp::IComponentUniquePtr CreateComponent() const = 0;
};


} // namespace icomp


