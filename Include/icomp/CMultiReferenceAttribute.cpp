#include "icomp/CMultiReferenceAttribute.h"


namespace icomp
{


// reimplemented (iser::IObject)

std::string CMultiReferenceAttribute::GetFactoryId() const
{
	return GetTypeName();
}


// static methods

std::string CMultiReferenceAttribute::GetTypeName()
{
	return "icomp::CMultiReferenceAttribute";
}


} // namespace icomp


