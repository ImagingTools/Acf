// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <icomp/IComponentStaticInfo.h>
#include <icomp/CComponentAddress.h>


namespace icomp
{


class IComponentListProvider: virtual public istd::IChangeable
{
public:
	enum ComponentTypeFlag
	{
		CTF_REAL = 1 << IComponentStaticInfo::CT_REAL,
		CTF_COMPOSITE = 1 << IComponentStaticInfo::CT_COMPOSITE,
		CTF_ALL = -1
	};

	typedef QSet<CComponentAddress> ComponentAddresses;

	/**
		Get addresses of known components.
	*/
	virtual ComponentAddresses GetComponentAddresses(int typeFlag = CTF_ALL) const = 0;
};


} // namespace icomp




