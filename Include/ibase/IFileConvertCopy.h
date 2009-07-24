#ifndef included_ibase_IFileConvertCopy
#define included_ibase_IFileConvertCopy


#include "istd/IPolymorphic.h"
#include "istd/CString.h"


namespace ibase
{


class IFileConvertCopy: virtual public istd::IPolymorphic
{
public:
	virtual bool CopyFile(const istd::CString& inputFilePath, const istd::CString& outputFilePath) const = 0;
};


} // namespace ibase


#endif // !included_ibase_IFileConvertCopy


