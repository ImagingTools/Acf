#ifndef ibase_IProcessExecutor_included
#define ibase_IProcessExecutor_included


#include "istd/IPolymorphic.h"
#include "istd/CString.h"

#include "isys/IApplicationEnvironment.h"


namespace ibase
{		


/**
	Main entry point of component-based application.

	\ingroup Main
*/
class IProcessExecutor: virtual public istd::IPolymorphic
{
public:
	/**
		Define execution environment for the process.
	*/
	virtual void SetEnvironment(const isys::IApplicationEnvironment& processEnvironment) = 0;

	/**
		Execute external process.
		\param applicationPath - Path to the application's file. 
		\param applicationArguments - Calling arguments for the application. 
	*/
	virtual int Execute(const istd::CString& executablePath, const istd::CStringList& processArguments) = 0;
};


} // namespace ibase


#endif // !ibase_IProcessExecutor_included


