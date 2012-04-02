#ifndef iqt_CFileSystem_included
#define iqt_CFileSystem_included


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
	// static members
	static QString GetNormalizedPath(const QString& path);
	static QString FindVariableValue(const QString& varName);
	static QString GetEnrolledPath(const QString& path);
};


} // namespace iqt


#endif // !iqt_CFileSystem_included


