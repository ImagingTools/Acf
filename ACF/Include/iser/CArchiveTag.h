#ifndef iser_CArchiveTag_included
#define iser_CArchiveTag_included


#include <string>

#include "iser/IVersionInfo.h"


namespace iser
{


class IArchive;


/**
	Process tag used to group data in archive stream.
*/
class CArchiveTag
{
public:
	CArchiveTag(	const ::std::string& id,
					const ::std::string& comment);

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
};



} // namespace iser



#endif // !iser_CArchiveTag_included



