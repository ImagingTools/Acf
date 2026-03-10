// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icomp/CReferenceAttribute.h>


namespace icomp
{


// public methods
	
CReferenceAttribute::CReferenceAttribute()
{
}


CReferenceAttribute::CReferenceAttribute(const CReferenceAttribute& attribute)
	:BaseClass(attribute)
{
}


CReferenceAttribute::CReferenceAttribute(const QByteArray& value)
	:BaseClass(value)
{
}


// reimplemented (iser::IObject)

QByteArray CReferenceAttribute::GetFactoryId() const
{
	return GetTypeName();
}


// static methods

QByteArray CReferenceAttribute::GetTypeName()
{
	return "Reference";
}


} // namespace icomp


