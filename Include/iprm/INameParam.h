#ifndef iprm_INameParam_included
#define iprm_INameParam_included


// ACF includes
#include "istd/CString.h"

#include "iser/ISerializable.h"


namespace iprm
{


/**
	Interface for a object containing its name.
*/
class INameParam: virtual public iser::ISerializable
{
public:
	/**
		Get the object name.
	*/
	virtual const istd::CString& GetName() const = 0;

	/**
		Set the object name.
	*/
	virtual void SetName(const istd::CString& name) = 0;

	/**
		Return \c true, if the name setting is enabled.
	*/
	virtual bool IsNameFixed() const = 0;
};


} // namespace iprm


#endif // !iprm_INameParam_included
