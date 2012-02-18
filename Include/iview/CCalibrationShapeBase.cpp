#include "iview/CCalibrationShapeBase.h"


#include "iview/ICalibrated.h"


namespace iview
{


CCalibrationShapeBase::CCalibrationShapeBase()
{
	SetTransformMode(STM_SHAPE);
}


// reimplemented (iview::IInteractiveShape)

void CCalibrationShapeBase::BeginDrag(const i2d::CVector2d& reference)
{
	I_ASSERT(IsDisplayConnected());
	I_ASSERT(m_calibPtr != NULL);

	const iview::IIsomorphicCalibration& isomorphCalib = GetIsomorphCalib();
	const i2d::CAffine2d& transform = GetLogToViewTransform();

	i2d::CVector2d logPos;
	isomorphCalib.GetApplyToLog(transform.GetInvertedApply(reference), logPos);
	BeginLogDrag(logPos);
}



void CCalibrationShapeBase::SetDragPosition(const i2d::CVector2d& position)
{
	I_ASSERT(IsDisplayConnected());
	I_ASSERT(m_calibPtr != NULL);

	const iview::IIsomorphicCalibration& isomorphCalib = GetIsomorphCalib();
	const i2d::CAffine2d& transform = GetLogToViewTransform();

	i2d::CVector2d logPos;
	isomorphCalib.GetApplyToLog(transform.GetInvertedApply(position), logPos);
	SetLogDragPosition(logPos);
}



// reimplement (iview::IVisualizable)

bool CCalibrationShapeBase::IsDisplayAccepted(const iview::IDisplay& display) const
{
	return BaseClass::IsDisplayAccepted(display) && (FindCalib(display) != NULL);
}



void CCalibrationShapeBase::OnConnectDisplay(iview::IDisplay* displayPtr)
{
	BaseClass::OnConnectDisplay(displayPtr);
	m_calibPtr = FindCalib(*displayPtr);
	I_ASSERT(m_calibPtr != NULL);
}



void CCalibrationShapeBase::OnDisconnectDisplay(iview::IDisplay* displayPtr)
{
	BaseClass::OnDisconnectDisplay(displayPtr);
	m_calibPtr = NULL;
}


// protected static methods

const IIsomorphicCalibration* CCalibrationShapeBase::FindCalib(const iview::IDisplay& display)
{
	const iview::IDisplay* displayPtr = &display;
	while (displayPtr != NULL){
		const ICalibrated* calibratedPtr = dynamic_cast<const ICalibrated*>(displayPtr);
		if (calibratedPtr != NULL){
			const IIsomorphicCalibration* isomorphCalibPtr = dynamic_cast<const iview::IIsomorphicCalibration*>(&calibratedPtr->GetCalibration());
			if (isomorphCalibPtr != NULL){
				return isomorphCalibPtr;
			}
		}

		displayPtr = displayPtr->GetParentDisplayPtr();
	}

	return NULL;
}



} // namespace iview



