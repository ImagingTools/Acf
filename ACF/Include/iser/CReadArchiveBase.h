#ifndef iser_CReadArchiveBase_included
#define iser_CReadArchiveBase_included


#include "iser/IArchive.h"
#include "iser/CReaderVersionInfo.h"


namespace iser
{


class CReadArchiveBase: public IArchive
{
public:
	// reimplemented (iser::IArchive)
	virtual bool IsStoring() const;
	virtual I_DWORD GetVersion(int versionId = IVersionInfo::UserVersionId) const;

protected:
	/**
		Constructor.
		\param	serializeHeader	if it is true, header will be serialized during contruction.
	*/
	CReadArchiveBase(bool serializeHeader);

	/**
		Serialize standard header.
		During serialization of header list of known versions will be loaded.
	*/
	virtual bool SerializeHeader();

private:
	CReaderVersionInfo m_versionInfo;
};


} // namespace iser


#endif // !iser_CReadArchiveBase_included


