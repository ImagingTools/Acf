#include "iser/CXmlDocumentInfoBase.h"


namespace iser
{


const istd::CString& CXmlDocumentInfoBase::GetElementSeparator()
{
	return s_elementSeparator;
}


// static attributes

istd::CString CXmlDocumentInfoBase::s_elementSeparator("br");


} // namespace iser


