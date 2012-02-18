#ifndef iview_CCalibrationShapeBase_included
#define iview_CCalibrationShapeBase_included


#include "iview/CInteractiveShapeBase.h"
#include "iview/IIsomorphicCalibration.h"


namespace iview
{


// TODO: Redesign it to ACF transformation concept.

/**
	Base class for all shapes using iview::IIsomorphicCalibration to transform between logical and physical positions.
*/
class CCalibrationShapeBase: public iview::CInteractiveShapeBase
{
public:
	typedef iview::CInteractiveShapeBase BaseClass;

	CCalibrationShapeBase();

	// reimplemented (iview::IInteractiveShape)
	virtual void BeginDrag(const i2d::CVector2d& position);
	virtual void SetDragPosition(const i2d::CVector2d& position);

	// reimplement (iview::IVisualizable)
	virtual bool IsDisplayAccepted(const iview::IDisplay& display) const;
	virtual void OnConnectDisplay(iview::IDisplay* displayPtr);
	virtual void OnDisconnectDisplay(iview::IDisplay* displayPtr);

protected:
	/**	Get access to calibration object.
	*/
	virtual const IIsomorphicCalibration& GetIsomorphCalib() const;

	static const IIsomorphicCalibration* FindCalib(const iview::IDisplay& display);

	// reimplemented (iview::TShapeBase)
	virtual int GetDisplayChangesMask();

private:
	const IIsomorphicCalibration* m_calibPtr;
};


// protected inline methods

inline const IIsomorphicCalibration& CCalibrationShapeBase::GetIsomorphCalib() const
{
	I_ASSERT(IsDisplayConnected());
	I_ASSERT(m_calibPtr != NULL);

	return *m_calibPtr;
}


// reimplemented (iview::TShapeBase)

inline int CCalibrationShapeBase::GetDisplayChangesMask()
{
	return BaseClass::GetDisplayChangesMask() | CF_CALIB;
}


} // namespace iview


#endif // !iview_CCalibrationShapeBase_included


