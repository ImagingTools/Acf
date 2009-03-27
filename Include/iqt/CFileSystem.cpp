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

	QString absolutePath = dir.absolutePath();

#ifdef _DEBUG
#ifdef _WIN32
	absolutePath.replace("$(ConfigurationName)", "DebugVC8");
#else // _WIN32
	absolutePath.replace("$(ConfigurationName)", "DebugXCD");
#endif // _WIN32
#else // _DEBUG
#ifdef _WIN32
	absolutePath.replace("$(ConfigurationName)", "ReleaseVC8");
#else // _WIN32
	absolutePath.replace("$(ConfigurationName)", "ReleaseXCD");
#endif // _WIN32
#endif // _DEBUG

	return iqt::GetCString(absolutePath);
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


// static members

istd::CString CFileSystem::GetEnrolledPath(const istd::CString& path)
{
	QString qtPath = iqt::GetQString(path);

#ifdef _DEBUG
#ifdef _WIN32
	qtPath.replace("$(ConfigurationName)", "DebugVC8");
#else // _WIN32
	qtPath.replace("$(ConfigurationName)", "DebugXCD");
#endif // _WIN32
#else // _DEBUG
#ifdef _WIN32
	qtPath.replace("$(ConfigurationName)", "ReleaseVC8");
#else // _WIN32
	qtPath.replace("$(ConfigurationName)", "ReleaseXCD");
#endif // _WIN32
#endif // _DEBUG

	return iqt::GetCString(qtPath);
}


} // namespace iqt


