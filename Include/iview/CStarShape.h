// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include <i2d/CPosition2d.h>
#include <iview/CInteractiveShapeBase.h>
#include <iview/CPinShape.h>


namespace iview
{


class CStarShape : public CPinShape
{
public:
	typedef CPinShape BaseClass;
	typedef CInteractiveShapeBase ShapeBaseClass;

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const override;

protected:
	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const override;
};


} // namespace iview



