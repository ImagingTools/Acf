#include "imath/CVector2d.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace imath
{


inline void CVector2d::Init(double angle, double length)
{
	SetX(::cos(angle) * length);
	SetY(::sin(angle) * length);
}


CVector2d CVector2d::GetOrthogonal() const
{
	return CVector2d(GetY(), -GetX());
}


CVector2d CVector2d::GetNormalized(double length) const
{
	CVector2d retVal;

    if (GetNormalized(retVal, length)){
		return retVal;
    }
	else{
        return CVector2d(length, 0.0);
	}
}


bool CVector2d::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag xTag("X", "X");

	bool retVal = archive.BeginTag(xTag);
	retVal = retVal && archive.Process(m_elements[0]);
	retVal = retVal && archive.EndTag(xTag);

	static iser::CArchiveTag yTag("Y", "Y");
	retVal = retVal && archive.BeginTag(yTag);
	retVal = retVal && archive.Process(m_elements[1]);
	retVal = retVal && archive.EndTag(yTag);

	return retVal;
}


} // namespace imath


