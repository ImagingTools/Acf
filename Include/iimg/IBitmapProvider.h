// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/IChangeable.h>
#include <iimg/IBitmap.h>


namespace iimg
{


/**
	Bitmap supplier allowing to access of produced bitmap.

	\ingroup ImageProcessing
	\ingroup Geometry
*/
class IBitmapProvider: virtual public istd::IChangeable
{
public:
	/**
		Get access to produced bitmap object.
		\return	pointer to bitmap instance if this bitmap is accessible, or NULL.
	*/
	virtual const iimg::IBitmap* GetBitmap() const = 0;
};


} // namespace iimg




