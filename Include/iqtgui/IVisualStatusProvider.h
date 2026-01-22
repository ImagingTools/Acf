// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
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




