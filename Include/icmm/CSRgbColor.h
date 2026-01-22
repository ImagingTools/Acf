// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <icmm/CRgb.h>
#include <icmm/CRgbColor.h>


namespace icmm
{


/**
	This class implements "regular" RGB Color class, BUT:
	makes it so that the ColorSpec is ALWAYS D65/2 (per the spec for SRGB)
*/
class CSRgbColor: public CRgbColor
{
public:
	typedef CRgbColor BaseClass;

	CSRgbColor(const icmm::CRgb& rgb);
};


} // namespace icmm


