#include "iser/CReadArchiveBase.h"


namespace iser
{


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
	bool retVal = BeginTag(s_headerTag);

	int versionIdsCount;
	retVal = retVal && BeginMultiTag(s_versionInfosTag, s_versionInfoTag, versionIdsCount);

	if (retVal){
		for (int i = 0; i < versionIdsCount; ++i){
			retVal = retVal && BeginTag(s_versionInfoTag);

			retVal = retVal && BeginTag(s_versionIdTag);
			int id;
			retVal = retVal && Process(id);
			retVal = retVal && EndTag(s_versionIdTag);

			retVal = retVal && BeginTag(s_versionNumberTag);
			I_DWORD version;
			retVal = retVal && Process(version);
			retVal = retVal && EndTag(s_versionNumberTag);

			retVal = retVal && BeginTag(s_versionDescriptionTag);
			::std::string description;
			retVal = retVal && Process(description);
			retVal = retVal && EndTag(s_versionDescriptionTag);

			if (!retVal){
				return false;
			}

			m_versionInfo.InsertVersionId(id, version, description);

			retVal = retVal && EndTag(s_versionInfoTag);
		}
	}

	retVal = retVal && EndTag(s_versionInfosTag);

	retVal = retVal && EndTag(s_headerTag);

	return retVal;
}


} // namespace iser


