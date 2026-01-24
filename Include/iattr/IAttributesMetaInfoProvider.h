// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <iattr/IAttributesProvider.h>
#include <iattr/IAttributeMetaInfo.h>


namespace iattr
{


/**
	Common interface for a container of properties.
*/
class IAttributesMetaInfoProvider: virtual public istd::IChangeable
{
public:
	/**
		Get all IDs of known attribute meta informations.
	*/
	virtual IAttributesProvider::AttributeIds GetAttributeMetaIds() const = 0;

	/**
		Get property with the given index.
	*/
	virtual const IAttributeMetaInfo* GetAttributeMetaInfo(const QByteArray& attributeId) const = 0;
};


} // namespace iattr




