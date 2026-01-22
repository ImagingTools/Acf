// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icmm/CColorTransformationProvider.h>


namespace icmm
{


// public static methods

icmm::IColorTransformation* CColorTransformationProvider::GetColorTransformation(int inputColorType, int outputColorType)
{
	if (inputColorType == HsvColor && outputColorType == RgbColor){
		return &s_hsvToRgbTransform;
	}

	if (inputColorType == RgbColor && outputColorType == CmykColor){
		return &s_rgbToCmykTransform;
	}

	if (inputColorType == CmykColor && outputColorType == RgbColor){
		return &s_cmykToRgbTransform;
	}

	return nullptr;
}


// private static members

icmm::CHsvToRgbTransformation CColorTransformationProvider::s_hsvToRgbTransform;
icmm::CRgbToCmykTransformation CColorTransformationProvider::s_rgbToCmykTransform;
icmm::CCmykToRgbTransformation CColorTransformationProvider::s_cmykToRgbTransform;


} // namespace icmm
