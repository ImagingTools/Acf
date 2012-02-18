#ifndef iview_CScaleCalibrationShape_included
#define iview_CScaleCalibrationShape_included


#include "iview/CNoneCalibrationShape.h"


namespace iview
{


// TODO: Redesign it to ACF transformation concept.

/**
	Shape object for perspective calibration.
	You need this shape to visualize calibration
	of type iview::CScaleCalibration on display console.
*/
class CScaleCalibrationShape: public iview::CNoneCalibrationShape
{
public:
	typedef iview::CNoneCalibrationShape BaseClass;

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const;
};


} // namespace iview


#endif // !iview_CScaleCalibrationShape_included


