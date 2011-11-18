#ifndef icomp_CMultiFactoryAttribute_included
#define icomp_CMultiFactoryAttribute_included


// STL includes
#include <string>

#include "icomp/CMultiReferenceAttribute.h"


namespace icomp
{


/**
	Special attribute used to store list of referenced component ID's.
*/
class CMultiFactoryAttribute: public CMultiReferenceAttribute
{
public:
	typedef CMultiReferenceAttribute BaseClass;

	enum DefaultAttributeFlags
	{
		DAF_OBLIGATORY = IAttributeStaticInfo::AF_OBLIGATORY | IAttributeStaticInfo::AF_MULTIPLE | IAttributeStaticInfo::AF_FACTORY,
		DAF_OPTIONAL = IAttributeStaticInfo::AF_NULLABLE | IAttributeStaticInfo::AF_MULTIPLE | IAttributeStaticInfo::AF_FACTORY
	};

	CMultiFactoryAttribute(){}
	CMultiFactoryAttribute(const CMultiFactoryAttribute& attribute):BaseClass(attribute){}
	CMultiFactoryAttribute(int elementsCount, std::string* valuesPtr):BaseClass(elementsCount, valuesPtr){}

	// reimplemented (iser::IObject)
	std::string GetFactoryId() const;

	// static methods
	static std::string GetTypeName();
};


} // namespace icomp


#endif // icomp_CMultiFactoryAttribute_included


