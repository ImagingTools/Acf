#ifndef iview_CPolylineCalibrationShape_included
#define iview_CPolylineCalibrationShape_included


#include "iview/CPolygonCalibrationShape.h"


namespace iview
{


// TODO: Redesign it to ACF transformation concept.
class CPolylineCalibrationShape: public CPolygonCalibrationShape
{
public:
	typedef CPolygonCalibrationShape BaseClass;

	CPolylineCalibrationShape();

	bool IsOrientationVisible() const;
	void SetOrientationVisible(bool state = true);

    // reimplemented (iview::CPolygonCalibrationShape)
	virtual bool IsTickerTouched(istd::CIndex2d position) const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);

protected:
	virtual void DrawCurve(QPainter& drawContext) const;
	virtual void DrawArea(QPainter& drawContext) const;
	virtual void DrawSelectionElements(QPainter& drawContext) const;
	virtual bool IsCurveTouched(istd::CIndex2d position) const;

private:
	bool m_isOrientationVisible;
};


inline bool CPolylineCalibrationShape::IsOrientationVisible() const
{
	return m_isOrientationVisible;
}


inline void CPolylineCalibrationShape::SetOrientationVisible(bool state)
{
	m_isOrientationVisible = state;
}


} // namespace iview


#endif // !iview_CPolylineCalibrationShape_included


