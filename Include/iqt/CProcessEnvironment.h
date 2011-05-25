#ifndef iqt_CProcessEnvironment_included
#define iqt_CProcessEnvironment_included


#include "isys/IProcessEnvironment.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	Qt-based implementation of application environment information.
*/
class CProcessEnvironment: virtual public isys::IProcessEnvironment
{
public:
	// reimplemented (isys::IProcessEnvironment)
	virtual int GetMainThreadId() const;
	virtual void Sleep(double seconds);
	virtual istd::CString GetTempDirPath() const;
	virtual istd::CString GetWorkingDirectory() const;
	virtual istd::CStringList GetApplicationArguments() const;
	virtual istd::CString GetModulePath(bool useApplicationModule = false, bool onlyDirectory = false) const;
	virtual EnvironmentVariables GetEnvironmentVariables() const;
	virtual void SetEnvironmentVariableValue(const istd::CString& variableName, const istd::CString& value);
};


} // namespace iqt


#endif // !iqt_CProcessEnvironment_included
