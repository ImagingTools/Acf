#ifndef iser_CBinaryReadArchiveBase_h_included
#define iser_CBinaryReadArchiveBase_h_included


#include "iser/CBinaryArchiveBase.h"


namespace iser
{


class CBinaryReadArchiveBase: public CBinaryArchiveBase
{
public:
	CBinaryReadArchiveBase();
	~CBinaryReadArchiveBase();

	virtual int tagSize() = 0;

	// reimplemented (iser::IArchive)
	virtual bool Process(CString& value);
	virtual bool Process(std::string& value);
	virtual bool IsStoring() const;
	virtual bool BeginTag(const CArchiveTag& tag, bool* skipFlagPtr = NULL);
	virtual bool EndTag();

protected:
	enum{
		MAX_STRING_LENGTH = 100000
	};

	struct TagStackElement
};


} // namespace iser


#endif // iser_CBinaryReadArchiveBase_h_included

