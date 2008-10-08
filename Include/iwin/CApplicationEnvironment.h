#ifndef iwin_CApplicationEnvironment_included
#define iwin_CApplicationEnvironment_included


#include "isys/IApplicationEnvironment.h"


namespace iwin
{


/**
	\ingroup iwin
*/
class CApplicationEnvironment: virtual public isys::IApplicationEnvironment
{
public:
	// reimplemented (isys::IApplicationEnvironment)
	virtual istd::CString GetTempDirPath() const;
	virtual istd::CString GetWorkingDirectory() const;
	virtual istd::CStringList GetApplicationArguments() const;
	virtual istd::CString GetModulePath(bool useApplicationModule = false, bool onlyDirectory = false) const;
};


} // namespace iwin


#endif // !iwin_CApplicationEnvironment_included
