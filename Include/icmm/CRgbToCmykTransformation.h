// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <icmm/IColorTransformation.h>


namespace icmm
{


/**
	Implementation of RGB-to-CMYK color transformation.
*/
class CRgbToCmykTransformation: public icmm::IColorTransformation
{
public:
	// reimplemented (icmm::IColorTransformation)
	virtual bool GetValueAt(const ArgumentType& argument, ResultType& result) const override;
	virtual ResultType GetValueAt(const ArgumentType& argument) const override;
};


} // namespace icmm


