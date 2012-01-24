#ifndef i2d_CLabel_included
#define i2d_CLabel_included


// ACF includes
#include "istd/CIndex2d.h"

#include "iser/CArchiveTag.h"

#include "i2d/CPosition2d.h"


namespace i2d
{


class CLabel: public i2d::CPosition2d
{
public:
	typedef i2d::CPosition2d BaseClass;

	CLabel(const i2d::CVector2d& position = i2d::CVector2d(0, 0), const istd::CString& labelText = "Label");

	virtual const istd::CString& GetText() const;
	virtual void SetText(const istd::CString& labelText);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	istd::CString m_text;
};


// inline methods

inline const istd::CString& CLabel::GetText() const
{
	return m_text;
}


} // namespace i2d


#endif // !i2d_CLabel_included


