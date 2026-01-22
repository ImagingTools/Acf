// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// ACF includes
#include <iqtgui/IVisualStatus.h>


namespace iqtgui
{


class IVisualStatusProvider: virtual public istd::IChangeable
{
public:
	virtual const IVisualStatus* GetVisualStatus() const = 0;
};


} // namespace iqtgui




