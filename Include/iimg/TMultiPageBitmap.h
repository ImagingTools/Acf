// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <iimg/CMultiPageBitmapBase.h>
#include <iimg/CGeneralBitmap.h>


namespace iimg
{

	
/**
	Generic implementation of a multi-page bitmap.

	\tparam	BitmapImpl	Type of the bitmap implementation.

	\ingroup ImageProcessing
	\ingroup Geometry
*/
template <class BitmapImpl>
class TMultiPageBitmap: public CMultiPageBitmapBase
{
public:
	typedef CMultiPageBitmapBase BaseClass;
	typedef BitmapImpl BitmapType;

	// reimplemented (istd::IChangeable)
	virtual istd::TUniqueInterfacePtr<istd::IChangeable> CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;

protected:
	// reimplemented (CMultiPageBitmapBase)
	virtual IBitmapUniquePtr CreateBitmap() const override;
};


// public methods

// reimplemented (istd::IChangeable)

template <class BitmapImpl>
istd::IChangeableUniquePtr TMultiPageBitmap<BitmapImpl>::CloneMe(CompatibilityMode mode) const
{
	istd::IChangeableUniquePtr clonedPtr(new TMultiPageBitmap<BitmapImpl>);
	if (clonedPtr->CopyFrom(*this, mode)){
		return clonedPtr;
	}

	return istd::IChangeableUniquePtr();
}


// protected methods

// reimplemented (CMultiPageBitmapBase)

template <class BitmapImpl>
IBitmapUniquePtr TMultiPageBitmap<BitmapImpl>::CreateBitmap() const
{
	return new BitmapImpl;
}


typedef TMultiPageBitmap<CGeneralBitmap> CGeneralMultiPageBitmap;


} // namespace iimg




