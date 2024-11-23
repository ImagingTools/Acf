#include <icmm/CCmykColorModel.h>


namespace icmm
{


// static public methods

IColorantList::ColorantId CCmykColorModel::GetBlack()
{
	return "Black";
}


// public methods

// reimplemented (icmm::IColorantList)

IColorantList::ColorantIds CCmykColorModel::GetColorantIds() const
{
	ColorantIds retVal = BaseClass::GetColorantIds();

	retVal += GetBlack();

	return retVal;
}


icmm::ColorantUsage CCmykColorModel::GetColorantUsage(const ColorantId& colorantId) const
{
	Q_ASSERT(GetColorantIds().contains(colorantId));

	if (colorantId == GetBlack()){
		return CU_BLACK;
	}

	return BaseClass::GetColorantUsage(colorantId);
}


} // namespace icmm


