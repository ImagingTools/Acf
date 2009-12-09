#include "iqt/CSystemEnvironment.h"


// Qt includes
#include <QUuid>
#include <QDir>


namespace iqt
{


// reimplemented (isys::ISystemEnvironment)

istd::CString CSystemEnvironment::GetUser() const
{
	return GetSystemVariableValue("USER");
}


istd::CString CSystemEnvironment::GetTempDirPath() const
{
	return iqt::GetCString(QDir::tempPath());
}


istd::CString CSystemEnvironment::GetUniqueIdentifier() const
{
	return iqt::GetCString(QUuid::createUuid().toString());
}


} // namespace isys


