#ifndef iview_CSplineShape_included
#define iview_CSplineShape_included



#include "iview/CPolylineShape.h"



namespace iview{



class CSplineShape: public CPolylineShape
{
public:
	typedef CPolylineShape BaseClass;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);

protected:
	virtual void DrawPolyBezier(QPainter& drawContext, const istd::CIndex2d* pointsPtr, int pointsCount) const;

	// reimplemented (iview::CPolygonShape)
	virtual i2d::CVector2d GetSegmentMiddle(int index) const;
	virtual void DrawCurve(QPainter& drawContext) const;
	virtual bool IsCurveTouched(istd::CIndex2d position) const;

	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const;
};



} // namespace iview



#endif // !iview_CSplineShape_included



