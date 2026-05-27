// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


#include <memory>


namespace istd
{


/**
	\deprecated Use std::unique_ptr instead.

	This header is provided for backward compatibility only.
	TDelPtr is now a type alias for std::unique_ptr.

	\ingroup Main
*/
template <class Type>
using TDelPtr = std::unique_ptr<Type>;


} // namespace istd



