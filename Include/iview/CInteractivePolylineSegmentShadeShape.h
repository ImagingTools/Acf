#ifndef iview_CInteractivePolylineSegmentShadeShape_included
#define iview_CInteractivePolylineSegmentShadeShape_included


// ACF includes
#include "i2d/TPolylineExPrimitive.h"



#include "iview/CInteractivePolylineShape.h"


namespace iview
{


class CInteractivePolylineSegmentShadeShape: public CInteractivePolylineShape
{
public:
	typedef CInteractivePolylineShape BaseClass;
	typedef i2d::TPolylineExPrimitive<double> Model;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);

protected:
	// reimplemented (iview::CInteractivePolygonShape)
	virtual void DrawCurve(QPainter& drawContext) const;
};


} // namespace iview


#endif // !iview_CInteractivePolylineSegmentShadeShape_included



