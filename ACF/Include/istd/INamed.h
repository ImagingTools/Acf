#ifndef istd_INamed_included
#define istd_INamed_included


#include "istd/CString.h"
#include "istd/IPolymorphic.h"


namespace istd
{


/**
	Interface for a named object
*/
class INamed: virtual public istd::IPolymorphic  
{
public:
	/**
		Gets the object name.
	*/
	virtual const istd::CString& GetName() const = 0;

	/**
		Sets the object name.
	*/
	virtual void SetName(const istd::CString& name) = 0;
};


} // namespace istd


#endif // istd_INamed_included
