#ifndef ibase_IApplicationInfo_included
#define ibase_IApplicationInfo_included


#include "istd/IPolymorphic.h"
#include "istd/CString.h"

#include "ibase/ibase.h"


#include "iser/IVersionInfo.h"


namespace ibase
{


/**
	This interface provides some information about the application.

	\ingroup Main
*/
class IApplicationInfo: public iser::IVersionInfo
{
public:
	enum ApplicationAttribute
	{
		AA_COMPANY_NAME,
		AA_PRODUCT_NAME,
		AA_APPLICATION_NAME,
		AA_APPLICATION_SUBNAME,
		AA_APPLICATION_TYPE,
		AA_APPLICATION_PATH,
		AA_LEGAL_COPYRIGHT,
		AA_USER = 100
	};

	/**
		Get value of application attribute.
		\param	attributeId	ID of application attribute \sa ApplicationAttribute.
	*/
	virtual istd::CString GetApplicationAttribute(int attributeId) const = 0;

	/**
		Encode version number to human readable form.
	*/
	virtual istd::CString GetEncodedVersionName(int versionId, I_DWORD versionNumber) const = 0;
};


} // namespace ibase


#endif // !ibase_IApplicationInfo_included


