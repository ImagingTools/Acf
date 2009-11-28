#ifndef ibase_CFileSystem_included
#define ibase_CFileSystem_included


#include "isys/IFileSystem.h"

#include "ibase/ibase.h"


namespace ibase
{


/**
	Implementation of simple file system information.
	This implementation is part of standard services.
*/
class CFileSystem: virtual public isys::IFileSystem
{
public:
	// reimplemented (isys::IFileSystem)
	virtual istd::CString GetNormalizedPath(const istd::CString& path) const;
	virtual istd::CString GetFileName(const istd::CString& filePath) const;
	virtual istd::CString GetBaseFileName(const istd::CString& filePath) const;
	virtual istd::CString GetDirPath(const istd::CString& filePath) const;
	virtual bool HasExtension(const istd::CString& filePath, const istd::CString& extension) const;
	virtual bool IsPresent(const istd::CString& filePath) const;
};


} // namespace ibase


#endif // !ibase_CFileSystem_included


