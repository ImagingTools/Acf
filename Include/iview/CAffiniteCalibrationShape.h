#ifndef iview_CAffiniteCalibrationShape_included
#define iview_CAffiniteCalibrationShape_included


#include "iview/CNoneCalibrationShape.h"


namespace iview
{


// TODO: Redesign it to ACF transformation concept.

/**
	Shape object for affinite calibration.
	You need this shape to visualize calibration
	of type iview::CAffiniteCalibration on display console.
*/
class CAffiniteCalibrationShape: public iview::CNoneCalibrationShape
{
public:
	typedef iview::CNoneCalibrationShape BaseClass;

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const;
};


} // namespace iview


#endif // !iview_CAffiniteCalibrationShape_included


