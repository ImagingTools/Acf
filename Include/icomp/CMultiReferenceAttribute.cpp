#include "icomp/CMultiReferenceAttribute.h"


namespace icomp{


// reimplemented (iser::IObject)

const std::string& CMultiReferenceAttribute::GetFactoryId() const
{
	return s_typeName;
}


// static methods

const std::string& CMultiReferenceAttribute::GetTypeName()
{
	return s_typeName;
}


// private static attributes

const std::string CMultiReferenceAttribute::s_typeName("icomp::CMultiReferenceAttribute");


} // namespace icomp


