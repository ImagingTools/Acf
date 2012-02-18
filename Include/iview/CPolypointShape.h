#ifndef iview_CPolypointShape_included
#define iview_CPolypointShape_included


#include "iview/CShapeBase.h"


namespace iview
{


class CPolypointShape: public CShapeBase
{
public:
	typedef CShapeBase BaseClass;

	CPolypointShape();

	bool IsSmallTickersMode() const;
	void SetSmallTickersMode(bool state = true);

	// reimplemented (iview::TShapeBase)
	virtual void CalcBoundingBox(i2d::CRect& result) const;

	// reimplemented (iview::IShape)
	virtual void Draw(QPainter& drawContext) const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);

protected:
	bool m_isSmallTickersMode;
};


} // namespace iview


#endif // !iview_CPolypointShape_included


