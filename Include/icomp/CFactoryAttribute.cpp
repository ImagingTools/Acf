#include "icomp/CFactoryAttribute.h"


namespace icomp{


// reimplemented (iser::IObject)

const std::string& CFactoryAttribute::GetFactoryId() const
{
	return s_typeName;
}


// static methods

const std::string& CFactoryAttribute::GetTypeName()
{
	return s_typeName;
}


// private static attributes

const std::string CFactoryAttribute::s_typeName("icomp::CFactoryAttribute");


} // namespace icomp


