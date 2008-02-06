#ifndef iser_CWriteArchiveBase_included
#define iser_CWriteArchiveBase_included


#include "iser/IArchive.h"


namespace iser
{


class CWriteArchiveBase: public IArchive
{
public:
	// reimplemented (iser::IArchive)
	virtual bool IsStoring() const;
	virtual I_DWORD GetVersion(int versionId = IVersionInfo::UserVersionId) const;

protected:
	/**
		Constructor.
		\param	versionInfoPtr	optional pointer to version info object.
		\param	serializeHeader	if it is true, header will be serialized during contruction.
	*/
	CWriteArchiveBase(const IVersionInfo* versionInfoPtr, bool serializeHeader);

	/**
		Serialize standard header.
		During serialization of header list of known versions will be loaded.
	*/
	virtual bool SerializeHeader();

private:
	const IVersionInfo* m_versionInfoPtr;
};


} // namespace iser


#endif // !iser_CWriteArchiveBase_included


