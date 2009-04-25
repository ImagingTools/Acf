#ifndef isys_IApplicationEnvironment_included
#define isys_IApplicationEnvironment_included


#include "isys/isys.h"


#include "istd/IPolymorphic.h"
#include "istd/CString.h"


namespace isys
{


/**
	Interface for access of some system dependent features and infos.
*/
class IApplicationEnvironment: virtual public istd::IPolymorphic
{
public:
	/**
		Returns the standard temp path, that will be used by the application.
	*/
	virtual istd::CString GetTempDirPath() const = 0;

	/**
		Returns the working directory of application.
	*/
	virtual istd::CString GetWorkingDirectory() const = 0;

	/**
		Returns the arguments of command line for running application process.
	*/
	virtual istd::CStringList GetApplicationArguments() const = 0;

	/**
		Returns the module file name. If the \c useApplicationModule is \c true returns the name of application file/directory,
		otherwise the file name or (directory path) of module in which context this function was called.
		if \c onlyDirectory is \c true only directory path will be returned.
	*/
	virtual istd::CString GetModulePath(bool useApplicationModule = false, bool onlyDirectory = false) const = 0;
};


} // namespace isys


#endif // !isys_IApplicationEnvironment_included
