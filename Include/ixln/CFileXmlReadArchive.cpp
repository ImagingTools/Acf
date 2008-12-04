#include "ixln/CFileXmlReadArchive.h"


#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>


namespace ixln
{


CFileXmlReadArchive::CFileXmlReadArchive(const std::string& filePath, bool serializeHeader)
{
	XMLCh* tmpData = xercesc::XMLString::transcode(filePath.c_str());
	xercesc::LocalFileInputSource inputSource(tmpData);
	xercesc::XMLString::release(&tmpData);

	Init(inputSource);

	if (serializeHeader){
		SerializeAcfHeader();
	}
}


} // namespace ixln


