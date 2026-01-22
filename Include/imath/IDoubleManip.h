// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <istd/IPolymorphic.h>
#include <imath/TIValueManip.h>


namespace imath
{


/**
	Interface for all manipulation using values represent as \c double.
*/
class IDoubleManip: virtual public TIValueManip<double>
{
public:
	/**
		Get number of digits after point.
	*/
	virtual int GetPrecision() const = 0;
};


} // namespace imath


