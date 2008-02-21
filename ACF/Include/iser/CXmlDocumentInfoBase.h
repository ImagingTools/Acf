#ifndef iser_CXmlDocumentInfoBase_included
#define iser_CXmlDocumentInfoBase_included


#include <string>

#include "iser/iser.h"


namespace iser
{


/**
	Base class for all XML implementations for ACF documents.
*/
class CXmlDocumentInfoBase
{
public:
	static const std::string& GetElementSeparator();

private:
	static std::string s_elementSeparator;
};


} // namespace iser


#endif // !iser_CXmlDocumentInfoBase_included


