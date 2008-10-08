#include "iwin/CApplicationEnvironment.h"

#include <windows.h>


EXTERN_C IMAGE_DOS_HEADER __ImageBase;


namespace iwin
{


// public methods
	
// reimplemented (isys::IApplicationEnvironment)

istd::CString CApplicationEnvironment::GetTempDirPath() const
{
	wchar_t tempPath[MAX_PATH] = {0};
	::GetTempPathW(MAX_PATH, tempPath);

	return istd::CString(tempPath);
}


istd::CString CApplicationEnvironment::GetWorkingDirectory() const
{
	wchar_t workingDirectory[MAX_PATH] = {0};

	if (::GetCurrentDirectoryW(MAX_PATH, workingDirectory) != 0){
		return istd::CString(workingDirectory);
	}

	return istd::CString();
}


istd::CStringList CApplicationEnvironment::GetApplicationArguments() const
{
	I_CRITICAL(); // TODO: implemented this feature

	return istd::CStringList();
}


istd::CString CApplicationEnvironment::GetModulePath(bool useApplicationModule /*= false*/, bool onlyDirectory /*= false*/) const
{
	CHAR moduleFileName[MAX_PATH] = {0};

	HINSTANCE moduleBasePtr = useApplicationModule ? NULL : (HINSTANCE)&__ImageBase;

	::GetModuleFileName(moduleBasePtr, moduleFileName, MAX_PATH);

	if (onlyDirectory){
		for (int i = MAX_PATH - 1; i >= 0; i--)   {   
			if (moduleFileName[i] == '\\' || moduleFileName[i] == '/'){   
				moduleFileName[i] = '\0';
				break;
			}
		}   	
	}

	return istd::CString(moduleFileName);
}


} // namespace iwin


