#ifndef ibase_IApplicationInfo_included
#define ibase_IApplicationInfo_included


#include "istd/IPolymorphic.h"
#include "istd/CString.h"

#include "ibase/ibase.h"


namespace ibase
{		


class IApplicationInfo: public istd::IPolymorphic
{
public:
	virtual istd::CString GetApplicationName() const = 0;
	virtual istd::CString GetApplicationPath() const = 0;
};


} // namespace ibase


#endif // ibase_IApplicationInfo_included


