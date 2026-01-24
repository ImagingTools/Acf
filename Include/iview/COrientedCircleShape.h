// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <iview/CCircleShape.h>


namespace iview
{


class COrientedCircleShape: public CCircleShape
{
public:
	typedef CCircleShape BaseClass;

	COrientedCircleShape();

	// reimplemented (iview::CCircleShape)
	virtual void Draw(QPainter& drawContext) const override;

protected:
	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask) override;
	
	// reimplemented (iview::CCircleShape)
	virtual i2d::CRect CalcBoundingBox() const override;
};


} // namespace iview




