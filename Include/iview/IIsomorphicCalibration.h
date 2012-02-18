#ifndef iview_IIsomorphicCalibration_included
#define iview_IIsomorphicCalibration_included


#include "iview/ICalibration.h"


namespace iview
{


// TODO: Replace it with ACF i2d::ITransformation2d.

/**
	This is common interface for all isomorphic calibrations.
	This calibration can transform beetween view and isomorphic coordinate systems.
	Please note, that is only possible for simple calibration methods.
*/
class IIsomorphicCalibration: virtual public ICalibration
{
public:
	/**
		Transform position to logical coordination system.
		\param	viewPos	position in view coordination system.
		\return	\c true, if it was possible to transform, otherwíse false.
	*/
	virtual CalcStatus GetApplyToLog(const i2d::CVector2d& viewPosition, i2d::CVector2d& result) const = 0;

	/**
		Transform position to view coordination system.
		\param	logPos	position in logical coordination system.
		\return \c true, if it was possible to transform, otherwise false.
	*/
	virtual CalcStatus GetApplyToView(const i2d::CVector2d& logPosition, i2d::CVector2d& result) const = 0;
};


} // namespace iview


#endif // !iview_IIsomorphicCalibration_included


