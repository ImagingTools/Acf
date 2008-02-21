#include "iser/CXmlDocumentInfoBase.h"


namespace iser
{


const std::string& CXmlDocumentInfoBase::GetElementSeparator()
{
	return s_elementSeparator;
}


// static attributes

std::string CXmlDocumentInfoBase::s_elementSeparator("br");


} // namespace iser


