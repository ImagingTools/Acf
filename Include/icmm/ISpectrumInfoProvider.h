// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/IChangeable.h>
#include <icmm/icmm.h>


namespace icmm
{


class ISpectrumInfo;


/**
	Common interface for a spectrum information provider.
*/
class ISpectrumInfoProvider: virtual public istd::IChangeable
{
public:
	virtual const ISpectrumInfo* GetSpectrumInfo() const = 0;
};


} // namespace icmm


