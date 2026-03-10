// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icomp/CFactoryAttribute.h>


namespace icomp
{


// public methods

CFactoryAttribute::CFactoryAttribute()
	:BaseClass()
{
}


CFactoryAttribute::CFactoryAttribute(const CFactoryAttribute& attribute)
	:BaseClass(attribute)

{
}


CFactoryAttribute::CFactoryAttribute(const QByteArray& value)
	:BaseClass(value)
{
}


// reimplemented (iser::IObject)

QByteArray CFactoryAttribute::GetFactoryId() const
{
	return GetTypeName();
}


// static methods

QByteArray CFactoryAttribute::GetTypeName()
{
	return "Factory";
}


} // namespace icomp


