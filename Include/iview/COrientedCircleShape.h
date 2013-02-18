#ifndef iview_COrientedCircleShape_included
#define iview_COrientedCircleShape_included


#include "iview/CCircleShape.h"


namespace iview
{


class COrientedCircleShape: public CCircleShape
{
public:
	typedef CCircleShape BaseClass;

	COrientedCircleShape();

	// reimplemented (iview::CCircleShape)
	virtual void Draw(QPainter& drawContext) const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	
	// reimplemented (iview::CCircleShape)
	virtual i2d::CRect CalcBoundingBox() const;
};


} // namespace iview


#endif // !iview_COrientedCircleShape_included


