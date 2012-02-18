#ifndef iview_CImageShape_included
#define iview_CImageShape_included


// ACF includes
#include "i2d/CAffine2d.h"

#include "iimg/IBitmap.h"

#include "iview/CShapeBase.h"


namespace iview
{


class CImageShape: public CShapeBase
{
public:
	typedef CShapeBase BaseClass;

	// reimplemented (iview::IShape)
	virtual void Draw(QPainter& drawContext) const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);

	// reimplemented (iview::CShapeBase)
	virtual void CalcBoundingBox(i2d::CRect& result) const;

protected:
	virtual void DrawBitmap(
				QPainter& painter,
				const iimg::IBitmap& bitmap,
				const i2d::CRect& bitmapArea,
				const i2d::CAffine2d& destTransform) const;
};


} // namespace iview


#endif // !iview_CImageShape_included



