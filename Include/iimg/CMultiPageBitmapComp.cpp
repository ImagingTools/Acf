// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iimg/CMultiPageBitmapComp.h>


namespace iimg
{


// protected methods

// reimplemented (CMultiPageBitmapBase)

IBitmapUniquePtr CMultiPageBitmapComp::CreateBitmap() const
{
	if (m_bitmapFactoryCompPtr.IsValid()){
		return m_bitmapFactoryCompPtr.CreateInstance();
	}

	return NULL;
}


} // namespace iimg


