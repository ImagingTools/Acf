// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <icmm/CHsvToRgbTransformation.h>
#include <icmm/CRgbToCmykTransformation.h>
#include <icmm/CCmykToRgbTransformation.h>


namespace icmm
{


/**
	Static provider of available color transformations.
*/
class CColorTransformationProvider: virtual public istd::IPolymorphic
{
public:
	enum ColorType
	{
		HsvColor = 0x0001,
		CmykColor = 0x0010,
		RgbColor = 0x0020,
		LabColor = 0x0040
	};

	CColorTransformationProvider();

	static IColorTransformation* GetColorTransformation(int inputColorType, int outputColorType);

private:
	static icmm::CHsvToRgbTransformation s_hsvToRgbTransform;
	static icmm::CRgbToCmykTransformation s_rgbToCmykTransform;
	static icmm::CCmykToRgbTransformation s_cmykToRgbTransform;
};

} // namespace icmm


