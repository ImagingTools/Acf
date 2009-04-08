#ifndef ixln_CFileXmlReadArchive_included
#define ixln_CFileXmlReadArchive_included


#include "ixln/CXmlReadArchiveBase.h"


namespace ixln
{


class CFileXmlReadArchive: public CXmlReadArchiveBase
{
public:
	typedef CXmlReadArchiveBase BaseClass;

	/**	Construct archive for specified file.
	 */
	explicit CFileXmlReadArchive(const istd::CString& filePath, bool serializeHeader = true, const iser::CArchiveTag& rootTag = s_acfRootTag);
};


} // namespace ixln


#endif // !ixln_CFileXmlReadArchive_included


