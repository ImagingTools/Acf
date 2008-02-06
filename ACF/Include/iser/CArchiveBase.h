#ifndef iser_CArchiveBase_included
#define iser_CArchiveBase_included


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iser
{


class CArchiveBase: public IArchive
{
public:
	// reimplemented (iser::IArchive)
	virtual bool IsTagSkippingSupported() const;
	virtual bool BeginMultiTag(const CArchiveTag& tag, const CArchiveTag& subTag, int& count);

protected:
	// list of standard tags used to serialize header
	static iser::CArchiveTag headerTag;
	static iser::CArchiveTag versionInfosTag;
	static iser::CArchiveTag versionInfoTag;
	static iser::CArchiveTag versionIdTag;
	static iser::CArchiveTag versionTag;
};


} // namespace iser


#endif // !iser_CArchiveBase_included


