#ifndef iqt_CFileSystem_included
#define iqt_CFileSystem_included


// STL includes
#include <map>

// Qt includes
#include <QtCore/QString>


namespace iqt
{


/**
	Implementation of file system information based on QT.
	This implementation is part of standard services if you use QT-based components.
*/
class CFileSystem
{
public:
	typedef std::map<QString, QString> EnvironmentVariables;

	// static members
	/**
		Get normalized path with enrolled system variables.
	*/
	static QString GetNormalizedPath(const QString& path);
	/**
		Get value of system variables.
	*/
	static QString FindVariableValue(const QString& varName);
	/**
		Get path with enrolled system variables.
	*/
	static QString GetEnrolledPath(const QString& path);
	static EnvironmentVariables GetEnvironmentVariables();
	/**
		Sleep current thread.
	*/
	static void Sleep(double seconds);
};


} // namespace iqt


#endif // !iqt_CFileSystem_included


