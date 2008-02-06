#include "iser/CReadArchiveBase.h"


namespace iser
{


CReadArchiveBase::CReadArchiveBase(const IVersionInfo* versionInfoPtr)
:	m_versionInfoPtr(versionInfoPtr)
{
}


// reimplemented (iser::IArchive)

bool CReadArchiveBase::IsStoring() const
{
	return false;
}


I_DWORD CReadArchiveBase::GetVersion(int versionId) const
{
	return m_versionInfo.GetVersion(versionId);
}


// protected methods

bool CReadArchiveBase::SerializeHeader()
{
	bool retVal = BeginTag(headerTag);

	int versionIdsCount;
	retVal = retVal && BeginMultiTag(versionInfosTag, versionInfoTag, versionIdsCount);

	if (retVal){
		for (int i = 0; i < versionIdsCount; ++i){
			retVal = retVal && BeginTag(versionInfoTag);

			retVal = retVal && BeginTag(versionIdTag);
			int id;
			retVal = retVal && Process(id);
			retVal = retVal && EndTag(versionIdTag);

			retVal = retVal && BeginTag(versionTag);
			I_DWORD version;
			retVal = retVal && Process(version);
			retVal = retVal && EndTag(versionTag);

			if (!retVal){
				return false;
			}

			m_versionInfo.InsertVersionId(id, version, "");

			retVal = retVal && EndTag(versionInfoTag);
		}
	}

	retVal = retVal && EndTag(versionInfosTag);

	retVal = retVal && EndTag(headerTag);

	return retVal;
}


} // namespace iser


#endif // !iser_CReadArchiveBase_included


