#ifndef icomp_CReferenceAttribute_included
#define icomp_CReferenceAttribute_included


// STL includes
#include <string>


// ACF includes
#include "icomp/TAttribute.h"


namespace icomp
{


/**
	Special attribute used to store referenced component ID.
*/
class CReferenceAttribute: public CStdStringAttribute
{
public:
	typedef CStdStringAttribute BaseClass;

	enum DefaultAttributeFlags
	{
		DAF_OBLIGATORY = IAttributeStaticInfo::AF_OBLIGATORY | IAttributeStaticInfo::AF_SINGLE | IAttributeStaticInfo::AF_REFERENCE,
		DAF_OPTIONAL = IAttributeStaticInfo::AF_NULLABLE | IAttributeStaticInfo::AF_SINGLE | IAttributeStaticInfo::AF_REFERENCE
	};

	CReferenceAttribute(){}
	CReferenceAttribute(const CReferenceAttribute& attribute):BaseClass(attribute){}
	explicit CReferenceAttribute(const std::string& value):BaseClass(value){}

	// reimplemented (iser::IObject)
	std::string GetFactoryId() const;

	// static methods
	static std::string GetTypeName();
};


} // namespace icomp


#endif // !icomp_CReferenceAttribute_included


