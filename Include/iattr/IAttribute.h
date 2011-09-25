#ifndef iattr_IAttribute_included
#define iattr_IAttribute_included


#include "iser/IObject.h"

#include "iattr/iattr.h"


namespace iattr
{


/**
	Common interface for an attribute.
*/
class IAttribute: virtual public iser::IObject
{
public:
	enum AttributeFlags
	{
		/**
			Attribute is persistent.
		*/
		AF_PERSISTENT = 0x1
	};

	/**
		Get default value for the attribute.
	*/
	virtual const iser::IObject* GetDefaultAttributeValue() const = 0;
};


} // namespace iattr


#endif // !iattr_IAttribute_included


