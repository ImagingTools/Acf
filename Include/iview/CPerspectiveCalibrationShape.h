#ifndef iview_CPerspectiveCalibrationShape_included
#define iview_CPerspectiveCalibrationShape_included


#include "iview/CNoneCalibrationShape.h"


namespace iview
{


// TODO: Redesign it to ACF transformation concept.

/**
	Shape object for perspective calibration.
	You need this shape to visualize calibration of type iview::CPerspectiveCalibration on the shape view.
*/
class CPerspectiveCalibrationShape: public iview::CNoneCalibrationShape
{
public:
	typedef iview::CNoneCalibrationShape BaseClass;

	// reimplemented (imod::IObserver)
	virtual void Invalidate();

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const;
};


} // namespace iview


#endif // !iview_CPerspectiveCalibrationShape_included


