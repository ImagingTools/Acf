#include "icomp/CMultiFactoryAttribute.h"


namespace icomp{


// reimplemented (iser::IObject)

const std::string& CMultiFactoryAttribute::GetFactoryId() const
{
	return s_typeName;
}


// static methods

const std::string& CMultiFactoryAttribute::GetTypeName()
{
	return s_typeName;
}


// private static attributes

const std::string CMultiFactoryAttribute::s_typeName("icomp::CMultiFactoryAttribute");


} // namespace icomp


