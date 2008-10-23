#ifndef zlib_CGzXmlReadArchive_included
#define zlib_CGzXmlReadArchive_included


// ZLIB includes
#include "zlib.h"

// ACF includes
#include "istd/CString.h"
#include "iser/CXmlReadArchiveBase.h"


namespace izlib
{


/**
	Archive implmentation writing compressed XML file.
*/
class CGzXmlReadArchive: public iser::CXmlReadArchiveBase 
{
public:
	typedef iser::CXmlReadArchiveBase BaseClass;

	CGzXmlReadArchive(const istd::CString& filePath, bool serializeHeader = true, const iser::CArchiveTag& rootTag = s_acfRootTag);
	virtual ~CGzXmlReadArchive();

	// reimplemented (iser::CXmlReadArchiveBase)
	virtual bool ReadToDelimeter(
				const std::string& delimeters,
				std::string& result,
				bool skipDelimeter = true,
				char* foundDelimeterPtr = NULL);

private:
	gzFile m_file;

	int m_lastReadChar;
	bool m_useLastReadChar;
};


} // namespace izlib


#endif // !zlib_CGzXmlReadArchive_included


