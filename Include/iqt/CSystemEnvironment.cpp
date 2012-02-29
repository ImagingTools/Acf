#include "iqt/CSystemEnvironment.h"


// Qt includes
#include <QtCore/QUuid>
#include <QtCore/QDir>


namespace iqt
{


// reimplemented (isys::ISystemEnvironment)

QString CSystemEnvironment::GetUser() const
{
	return GetSystemVariableValue("USER");
}


QString CSystemEnvironment::GetTempDirPath() const
{
	return QDir::tempPath();
}


QString CSystemEnvironment::GetUniqueIdentifier() const
{
	return QUuid::createUuid().toString();
}


} // namespace isys


