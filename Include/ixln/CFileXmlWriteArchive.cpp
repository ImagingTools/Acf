#include "ixln/CFileXmlWriteArchive.h"


#include <xercesc/framework/LocalFileFormatTarget.hpp>


namespace ixln
{


CFileXmlWriteArchive::CFileXmlWriteArchive(
			const istd::CString& filePath,
			const iser::IVersionInfo* versionInfoPtr,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
:	BaseClass(versionInfoPtr, rootTag)
{
	XMLCh* tmpData = xercesc::XMLString::transcode(filePath.ToString().c_str());
	Init(new xercesc::LocalFileFormatTarget(tmpData));
	xercesc::XMLString::release(&tmpData);


	if (serializeHeader){
		SerializeAcfHeader();
	}
}


} // namespace ixln


