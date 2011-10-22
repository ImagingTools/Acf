#include "icomp/CReferenceAttribute.h"


namespace icomp
{


// reimplemented (iser::IObject)

std::string CReferenceAttribute::GetFactoryId() const
{
	return GetTypeName();
}


// static methods

std::string CReferenceAttribute::GetTypeName()
{
	return "icomp::CReferenceAttribute";
}


} // namespace icomp


