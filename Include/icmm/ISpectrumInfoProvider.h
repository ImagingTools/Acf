// SPDX-License-Identifier: LGPL-2.1-only
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


