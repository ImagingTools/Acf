#ifndef iser_CBinaryWriteArchiveBase_h_included
#define iser_CBinaryWriteArchiveBase_h_included


#include "iser/CBinaryArchiveBase.h"


namespace iser
{


class CBinaryWriteArchiveBase: public CBinaryArchiveBase
{
public:
	CBinaryWriteArchiveBase();
	~CBinaryWriteArchiveBase();

	// reimplemented (iser::IArchive)
	virtual bool Process(CString& value);
	virtual bool Process(std::string& value);
	virtual bool BeginTag(const CArchiveTag& tag, bool* skipFlagPtr = NULL);
	virtual bool EndTag();
	virtual bool IsStoring() const;
};


} // namespace iser


#endif // iser_CBinaryWriteArchiveBase_h_included

