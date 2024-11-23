#pragma once


// ACF includes
#include <icmm/CCmyColorModel.h>


namespace icmm
{


class CCmykColorModel: public CCmyColorModel
{
public:
	typedef CCmyColorModel BaseClass;

	static ColorantId GetBlack();

	// reimplemented (icmm::IColorantList)
	virtual ColorantIds GetColorantIds() const override;
	icmm::ColorantUsage GetColorantUsage(const ColorantId& colorantId) const override;
};


} // namespace icmm


