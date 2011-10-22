#include "icomp/CMultiFactoryAttribute.h"


namespace icomp
{


// reimplemented (iser::IObject)

std::string CMultiFactoryAttribute::GetFactoryId() const
{
	return GetTypeName();
}


// static methods

std::string CMultiFactoryAttribute::GetTypeName()
{
	return "icomp::CMultiFactoryAttribute";
}


} // namespace icomp


