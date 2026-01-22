// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iview/CScreenTransformationProviderComp.h>


namespace iview
{

CScreenTransformationProviderComp::CScreenTransformationProviderComp()
{
	m_transformPtr.SetPtr(new iview::CScreenTransform);
	m_transformPtr->Reset();
}

istd::TSmartPtr<iview::CScreenTransform> CScreenTransformationProviderComp::GetTransformation() const
{
	return m_transformPtr;
}


} // namespace iview


