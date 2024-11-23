#pragma once


// ACF includes
#include <icmm/CSubstractiveColorModelBase.h>


namespace icmm
{


class CCmyColorModel: public CSubstractiveColorModelBase
{
public:
	// reimplemented (icmm::IColorantList)
	virtual ColorantIds GetColorantIds() const override;
	virtual ColorantUsage GetColorantUsage(const ColorantId& colorantId) const override;
};


// public methods

// reimplemented (icmm::IColorantList)

IColorantList::ColorantIds CCmyColorModel::GetColorantIds() const
{
	return {"Cyan", "Magenta", "Yellow"};
}


icmm::ColorantUsage CCmyColorModel::GetColorantUsage(const ColorantId& colorantId) const
{
	Q_ASSERT(GetColorantIds().contains(colorantId));

	if (colorantId == "Cyan"){
		return CU_CYAN;
	}
	else if (colorantId == "Magenta"){
		return CU_MAGENTA;
	}
	else if (colorantId == "Yellow"){
		return CU_YELLOW;
	}

	return CU_NONE;
}


} // namespace icmm


