#include "iqt/CApplicationEnvironment.h"


#include <QDir>
#include <QCoreApplication>

#include "iqt/iqt.h"


namespace iqt
{


// public methods
	
// reimplemented (isys::IApplicationEnvironment)

istd::CString CApplicationEnvironment::GetTempDirPath() const
{
	return iqt::GetCString(QDir::currentPath());
}


istd::CString CApplicationEnvironment::GetWorkingDirectory() const
{
	return iqt::GetCString(QDir::tempPath());
}


istd::CStringList CApplicationEnvironment::GetApplicationArguments() const
{
	return iqt::GetCStringList(QCoreApplication::arguments());
}


istd::CString CApplicationEnvironment::GetModulePath(bool /*useApplicationModule = false*/, bool onlyDirectory /*= false*/) const
{
	if (!onlyDirectory){
		return iqt::GetCString(QCoreApplication::applicationFilePath());
	}

	return iqt::GetCString(QCoreApplication::applicationDirPath());
}


} // namespace iqt


