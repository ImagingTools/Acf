#include "ixln/CFileXmlReadArchive.h"


#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>


namespace ixln
{


CFileXmlReadArchive::CFileXmlReadArchive(const istd::CString& filePath, bool serializeHeader, const iser::CArchiveTag& rootTag)
:	BaseClass(rootTag)
{
	XMLCh* tmpData = xercesc::XMLString::transcode(filePath.ToString().c_str());
	xercesc::LocalFileInputSource inputSource(tmpData);
	xercesc::XMLString::release(&tmpData);

	Init(inputSource);

	if (serializeHeader){
		SerializeAcfHeader();
	}
}


} // namespace ixln


