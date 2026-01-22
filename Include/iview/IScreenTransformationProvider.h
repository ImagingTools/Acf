// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// ACF includes
#include <istd/IPolymorphic.h>
#include <istd/TSmartPtr.h>
#include <iview/CScreenTransform.h>


namespace iview
{

class IScreenTransformationProvider: virtual public istd::IPolymorphic
{
public:
	virtual istd::TSmartPtr<iview::CScreenTransform> GetTransformation() const = 0;
};


} // namespace iview




