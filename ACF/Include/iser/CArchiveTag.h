#ifndef INCLUDED_iser_CArchiveTag_h
#define INCLUDED_iser_CArchiveTag_h


#include <string>

#include "iser/iser.h"


namespace iser
{


/**
	Achive tag used to group data in archive stream.
*/
class CArchiveTag
{
public:
	CArchiveTag(	const ::std::string& id,
					const ::std::string& comment,
					int versioningType = IArchive::VT_USER_APPLICATION,
					I_DWORD fromVersion = 0,
					I_DWORD toVersion = 0xffffffff);

	/**
		Check if this tag is active for specified archive.
	*/
	bool AcceptArchive(const IArchive& archive) const;

	/**
		Get ID of this tag.
	*/
	const ::std::string& GetId() const;
	/**
		Get automatic generated binary ID.
	*/
	I_DWORD GetBinaryId() const;
	/**
		Get comment of this tag.
		Comments are human readable and are used for diagnostic.
	*/
	const ::std::string& GetComment() const;

private:
	::std::string m_id;
	I_DWORD m_binaryId;

	::std::string m_comment;

	int m_versioningType;

	I_DWORD m_fromVersion;
	I_DWORD m_toVersion;
};



} // namespace iser



#endif //!INCLUDED_iser_CArchiveTag_h



