#include "iser/CArchiveTag.h"




namespace iser
{



CArchiveTag::CArchiveTag(
				const ::std::string& id,
				const ::std::string& comment,
				int versioningType,
				I_DWORD fromVersion,
				I_DWORD toVersion)
	:	m_id(id),
		m_comment(comment),
		m_versioningType(versioningType),
		m_fromVersion(fromVersion),
		m_toVersion(toVersion)
{
	m_binaryId = 0;
	int idSize = int(m_id.size());
	for (int i = 0; i < idSize; ++i){
		int character = m_id[i];
		int character2 = character * character;
		m_binaryId += (character << i) ^ (character * character >> i);
	}
}



bool CArchiveTag::AcceptArchive(const IArchive& archive) const
{
	I_DWORD version = archive.GetVersion(m_versioningType);

	return (version >= m_fromVersion) && (version <= m_toVersion);
}



const ::std::string& CArchiveTag::GetId() const
{
	return m_id;
}



I_DWORD CArchiveTag::GetBinaryId() const
{
	return m_binaryId;
}



const ::std::string& CArchiveTag::GetComment() const
{
	return m_comment;
}



} // namespace iser



