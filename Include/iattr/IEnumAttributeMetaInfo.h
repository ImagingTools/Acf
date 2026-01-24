// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


#include <iattr/IAttributeMetaInfo.h>


namespace iattr
{


/**
	Represents constraints of properties allowing values from enumerated set.
*/
class IEnumAttributeMetaInfo: virtual public IAttributeMetaInfo
{
public:
	/**
		Checks if other then enumerated values are allowed.
		If it is enabled option list represents presets only. In this case general constraints can be taken using \c GetOtherValueConstraints.
	*/
	virtual bool IsAnyValueAllowed() const = 0;
	/**
		Gets number of enumerations.
	*/
	virtual int GetEnumsCount() const = 0;
	/**
		Gets description of single enumeration.
		\param	index	index of enumeration.
	*/
	virtual QString GetValueDescription(const iser::IObject& value) const = 0;
	/**
		Gets single enumeration.
	*/
	virtual const iser::IObject& GetEnum(int index) const = 0;
};


} // namespace iattr




