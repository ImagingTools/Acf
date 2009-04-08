#include "iser/CArchiveBase.h"


namespace iser
{


// reimplemented (iser::IArchive)

bool CArchiveBase::IsTagSkippingSupported() const
{
	return false;
}


bool CArchiveBase::BeginMultiTag(const CArchiveTag& tag, const CArchiveTag& /*subTag*/, int& count)
{
	bool retVal = BeginTag(tag);

	retVal = retVal && Process(count);

	return retVal;
}

	
const CArchiveTag& CArchiveBase::GetAcfRootTag()
{
	static CArchiveTag retVal("Acf", "Root tag of ACF");
	
	return retVal;
}


// static attributes

CArchiveTag CArchiveBase::s_acfRootTag("Acf", "Root tag of ACF");
CArchiveTag CArchiveBase::s_headerTag("AcfHeader", "Header of ACF archive");
CArchiveTag CArchiveBase::s_versionInfosTag("VersionInfos", "List of version infos");
CArchiveTag CArchiveBase::s_versionInfoTag("Version", "Version info");
CArchiveTag CArchiveBase::s_versionIdTag("Id", "Version ID");
CArchiveTag CArchiveBase::s_versionNumberTag("Number", "Version number");
CArchiveTag CArchiveBase::s_versionDescriptionTag("Description", "Version description");


} // namespace iser


