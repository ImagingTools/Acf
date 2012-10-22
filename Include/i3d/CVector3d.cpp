#include "i3d/CVector3d.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace i3d
{


CVector3d CVector3d::GetCrossProduct(const imath::TVector<3>& vector) const
{
	return CVector3d(
				GetY() * vector[2] - GetZ() * vector[1],
				GetZ() * vector[0] - GetX() * vector[2],
				GetX() * vector[1] - GetY() * vector[0]);
}


CVector3d CVector3d::GetNormalized(double length) const
{
	CVector3d retVal;

	if (GetNormalized(retVal, length)){
		return retVal;
	}

	return CVector3d(length, 0.0, 0.0);
}


} // namespace i3d


