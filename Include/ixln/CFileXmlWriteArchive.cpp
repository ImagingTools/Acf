#include "ixln/CFileXmlWriteArchive.h"


#include <xercesc/framework/LocalFileFormatTarget.hpp>


namespace ixln
{


CFileXmlWriteArchive::CFileXmlWriteArchive(
					const std::string& filePath,
					const iser::IVersionInfo* versionInfoPtr,
					bool serializeHeader)
:	BaseClass(versionInfoPtr)
{
	XMLCh* tmpData = xercesc::XMLString::transcode(filePath.c_str());
	Init(new xercesc::LocalFileFormatTarget(tmpData));
	xercesc::XMLString::release(&tmpData);


	if (serializeHeader){
		SerializeAcfHeader();
	}
}


} // namespace ixln


