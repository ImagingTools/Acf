#ifndef ibase_IObject_included
#define ibase_IObject_included


#include "iser/ISerializable.h"

#include "ibase/ibase.h"


namespace ibase
{


/**
	Common interface for factorisable model objects.
*/
class IObject: virtual public iser::ISerializable
{
public:
	virtual istd::CString& GetFactoryId() const;
};


// public methods 

inline istd::CString& IObject::GetFactoryId() const
{
	static istd::CString emptyId;

	return emptyId;
}


} // namespace ibase


#endif // !ibase_IObject_included

