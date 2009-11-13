#ifndef icomp_IRegistryLoader_included
#define icomp_IRegistryLoader_included


#include "icomp/IRegistry.h"


namespace icomp
{

/**
	Interface for a ACF registry loader.
*/
class IRegistryLoader: virtual public istd::IPolymorphic
{
public:
	/**
		Configure environment for specified file path.
		If this file path is invalid or empty, standard configuration file will be used.
	*/
	virtual bool ConfigureEnvironment(const istd::CString& configFilePath = istd::CString()) = 0;
	/**
		Get file path of real used config file.
		If no config file was used, it returns empty string.
	*/
	virtual istd::CString GetConfigFilePath() const = 0;

	/**
		Get an ACF registry from a given file.
	*/
	virtual const icomp::IRegistry* GetRegistryFromFile(const istd::CString& path) const = 0;
};
	

} // namespace icomp


#endif // !icomp_IRegistryLoader_included


