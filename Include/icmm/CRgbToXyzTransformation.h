// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <icmm/IColorTransformation.h>


namespace icmm
{


/**
	Implementation of RGB-to-XYZ color transformation.
	2-degree Observer and D65-illumination are used.
*/
class CRgbToXyzTransformation: public icmm::IColorTransformation
{
public:
	// reimplemented (icmm::IColorTransformation)
	virtual bool GetValueAt(const icmm::CVarColor& argument, icmm::CVarColor& result) const override;
	virtual icmm::CVarColor GetValueAt(const icmm::CVarColor& argument) const override;
};


} // namespace icmm


