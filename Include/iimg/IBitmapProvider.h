// SPDX-License-Identifier: LGPL-2.1-only
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




