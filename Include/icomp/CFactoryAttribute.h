#ifndef icomp_CFactoryAttribute_included
#define icomp_CFactoryAttribute_included


// STL includes
#include <string>


// ACF includes
#include "icomp/CReferenceAttribute.h"


namespace icomp
{


/**
	Special attribute used to store referenced component ID.
*/
class CFactoryAttribute: public CReferenceAttribute
{
public:
	typedef CReferenceAttribute BaseClass;

	enum DefaultAttributeFlags
	{
		DAF_OBLIGATORY = IAttributeStaticInfo::AF_OBLIGATORY | IAttributeStaticInfo::AF_FACTORY,
		DAF_OPTIONAL = IAttributeStaticInfo::AF_NULLABLE | IAttributeStaticInfo::AF_FACTORY
	};

	CFactoryAttribute(){}
	CFactoryAttribute(const CFactoryAttribute& attribute):BaseClass(attribute){}
	explicit CFactoryAttribute(const std::string& value):BaseClass(value){}

	// reimplemented (iser::IObject)
	std::string GetFactoryId() const;

	// static methods
	static std::string GetTypeName();
};


} // namespace icomp


#endif // !icomp_CFactoryAttribute_included


