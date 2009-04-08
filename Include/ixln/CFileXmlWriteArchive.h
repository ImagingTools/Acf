#ifndef ixln_CFileXmlWriteArchive_included
#define ixln_CFileXmlWriteArchive_included


#include "ixln/CXmlWriteArchiveBase.h"


namespace ixln
{


class CFileXmlWriteArchive: public CXmlWriteArchiveBase
{
public:
	typedef CXmlWriteArchiveBase BaseClass;

	explicit CFileXmlWriteArchive(
				const istd::CString& filePath,
				const iser::IVersionInfo* versionInfoPtr = NULL,
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);
};


} // namespace ixln


#endif // !ixln_CFileXmlWriteArchive_included


