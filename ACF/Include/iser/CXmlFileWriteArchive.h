#ifndef iser_CXmlFileWriteArchive_included
#define iser_CXmlFileWriteArchive_included


#include <fstream>

#include "iser/CXmlWriteArchiveBase.h"


namespace iser
{


/**
	Simple implementation of archive for writing in XML format.
	This implementation doesn't use any parser.
*/
class CXmlFileWriteArchive: public CXmlWriteArchiveBase
{
public:
	typedef CXmlWriteArchiveBase BaseClass;

	CXmlFileWriteArchive(
				const istd::CString& fileName,
				const IVersionInfo* versionInfoPtr = NULL,
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);
	virtual ~CXmlFileWriteArchive();

protected:
	// reimplemented (iser::CXmlWriteArchiveBase)
	virtual bool WriteString(const ::std::string& value);

private:
	::std::ofstream m_stream;
};


} // namespace iser


#endif // !iser_CXmlFileWriteArchive_included


