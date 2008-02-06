#include "iser/CArchiveBase.h"


namespace iser
{


// reimplemented (iser::IArchive)

bool CArchiveBase::IsTagSkippingSupported() const
{
	return false;
}


bool CArchiveBase::BeginMultiTag(const CArchiveTag& tag, const CArchiveTag& subTag, int& count)
{
	bool retVal = BeginTag(tag);

	retVal = retVal && Process(count);

	return retVal;
}


// static attributes

iser::CArchiveTag CArchiveBase::headerTag("AcfHeader", "Header of ACF archive");
iser::CArchiveTag CArchiveBase::versionInfosTag("VersionInfos", "List of version infos");
iser::CArchiveTag CArchiveBase::versionInfoTag("Info", "Version info");
iser::CArchiveTag CArchiveBase::versionIdTag("Id", "Version ID");
iser::CArchiveTag CArchiveBase::versionTag("Version", "Version number");


} // namespace iser


