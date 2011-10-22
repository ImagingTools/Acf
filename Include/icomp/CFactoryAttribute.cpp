#include "icomp/CFactoryAttribute.h"


namespace icomp
{


// reimplemented (iser::IObject)

std::string CFactoryAttribute::GetFactoryId() const
{
	return GetTypeName();
}


// static methods

std::string CFactoryAttribute::GetTypeName()
{
	return "icomp::CFactoryAttribute";
}


} // namespace icomp


