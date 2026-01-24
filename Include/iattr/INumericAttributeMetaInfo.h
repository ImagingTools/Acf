// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


#include <iattr/IAttributeMetaInfo.h>


namespace iattr
{


/**
	Represents constraints of properties allowing values from enumerated set.
*/
class INumericAttributeMetaInfo: virtual public IAttributeMetaInfo
{
public:
	/**
		Get minimal allowed value for some attribute.
	*/
	virtual bool GetMinimalValue(iser::IObject& result) const = 0;
	/**
		Get maximal allowed value for some attribute.
	*/
	virtual bool GetMaximalValue(iser::IObject& result) const = 0;
};


} // namespace iattr




