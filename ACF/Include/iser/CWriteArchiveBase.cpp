#include "iser/CWriteArchiveBase.h"


namespace iser
{


CWriteArchiveBase::CWriteArchiveBase(const IVersionInfo* versionInfoPtr)
:	m_versionInfoPtr(versionInfoPtr)
{
}


// reimplemented (iser::IArchive)

bool CWriteArchiveBase::IsStoring() const
{
	return true;
}


I_DWORD CWriteArchiveBase::GetVersion(int versionId) const
{
	if (versionInfoPtr != NULL){
		return versionInfoPtr->GetVersion(versionId);
	}

	return IVersionInfo::UnknownVersion;
}


// protected methods

bool CReadArchiveBase::SerializeHeader()
{
	bool retVal = BeginTag(headerTag);

	IVersionInfo::VersionIds ids;

	if (versionInfoPtr != NULL){
		ids = versionInfoPtr->GetVersionIds();
	}

	int versionIdsCount = int(ids.size());

	retVal = retVal && BeginMultiTag(versionInfosTag, versionInfoTag, versionIdsCount);

	for (		IVersionInfo::VersionIds::iterator iter = ids.begin();
				retVal && (iter != ids.end());
				++iter){
		I_ASSERT(versionInfoPtr != NULL);

		retVal = retVal && BeginTag(versionInfoTag);

		retVal = retVal && BeginTag(versionIdTag);
		int id = *iter;
		retVal = retVal && Process(id);
		retVal = retVal && EndTag(versionIdTag);

		retVal = retVal && BeginTag(versionTag);
		I_DWORD version = versionInfoPtr->GetVersion(id);
		retVal = retVal && Process(version);
		retVal = retVal && EndTag(versionTag);

		retVal = retVal && EndTag(versionInfoTag);
	}

	retVal = retVal && EndTag(versionInfosTag);

	retVal = retVal && EndTag(headerTag);

	return retVal;
}


} // namespace iser


#endif // !iser_CReadArchiveBase_included


