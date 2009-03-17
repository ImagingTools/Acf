#include "iqt/CFileSystem.h"


// QT includes
#include <QString>
#include <QFileInfo>
#include <QDir>


namespace iqt
{


// reimplemented (isys::IFileSystem)

istd::CString CFileSystem::GetNormalizedPath(const istd::CString& path) const
{
	QDir dir(iqt::GetQString(path));

	return iqt::GetCString(dir.absolutePath());
}


istd::CString CFileSystem::GetFileName(const istd::CString& filePath) const
{
	QFileInfo fileInfo(iqt::GetQString(filePath));

	return iqt::GetCString(fileInfo.absoluteFilePath());
}


bool CFileSystem::HasExtension(const istd::CString& filePath, const istd::CString& extension) const
{
	return iqt::GetQString(filePath).endsWith(iqt::GetQString(extension));
}


bool CFileSystem::IsPresent(const istd::CString& filePath) const
{
	QFileInfo fileInfo(iqt::GetQString(filePath));

	return fileInfo.exists();
}


} // namespace iqt


