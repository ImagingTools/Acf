// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <imath/CSampledFunction2d.h>
#include <iimg/CBitmap.h>


namespace iimg
{


/**
	Union of a bitmap objects and its samples represented as a 2D sampled function.

	\ingroup ImageProcessing
	\ingroup Geometry
*/
class CBitmapSurface: public iimg::CBitmap, public imath::CSampledFunction2d
{
public:
	typedef iimg::CBitmap BaseClass;
	typedef imath::CSampledFunction2d BaseClass2;

	CBitmapSurface();
	CBitmapSurface(const CBitmapSurface& bitmap);

	// reimplemented (istd::IChangeable)
	virtual void OnEndChanges(const ChangeSet& changeSet) override;

private:
	void CreateSurfaceFromBitmap();
};


} // namespace iimg




