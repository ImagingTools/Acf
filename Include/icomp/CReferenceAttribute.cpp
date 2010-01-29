#include "icomp/CReferenceAttribute.h"


namespace icomp{


// reimplemented (iser::IObject)

const std::string& CReferenceAttribute::GetFactoryId() const
{
	return s_typeName;
}


// static methods

const std::string& CReferenceAttribute::GetTypeName()
{
	return s_typeName;
}


// private static attributes

const std::string CReferenceAttribute::s_typeName("icomp::CReferenceAttribute");


} // namespace icomp


