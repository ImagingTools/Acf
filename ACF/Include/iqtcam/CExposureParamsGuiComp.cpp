#include "iqtcam/CExposureParamsGuiComp.h"


#include "istd/TChangeNotifier.h"

#include "icam/IExposureConstraints.h"


namespace iqtcam
{


// reimplemented (iqt::TGuiObserverWrap)

void CExposureParamsGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	QObject::connect(ShutterTimeSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(DelayTimeSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(EenDelayTimeSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	bool isShutterTimeSupported = false;
	bool isDelayTimeSupported = false;
	bool isEenDelayTimeSupported = false;

	const icam::IExposureParams* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const icam::IExposureConstraints* constraintsPtr = objectPtr->GetConstraints();
		if (constraintsPtr != NULL){
			isShutterTimeSupported = !constraintsPtr->GetShutterTimeRange().IsEmpty();
			isDelayTimeSupported = !constraintsPtr->GetDelayTimeRange().IsEmpty();
			isEenDelayTimeSupported = !constraintsPtr->GetEenDelayRange().IsEmpty();
		}
	}

	ShutterTimeLabel->setVisible(isShutterTimeSupported);
	ShutterTimeSB->setVisible(isShutterTimeSupported);
	ShutterTimeUnitLabel->setVisible(isShutterTimeSupported);

	DelayTimeLabel->setVisible(isDelayTimeSupported);
	DelayTimeSB->setVisible(isDelayTimeSupported);
	DelayTimeUnitLabel->setVisible(isDelayTimeSupported);

	EenDelayTimeLabel->setVisible(isEenDelayTimeSupported);
	EenDelayTimeSB->setVisible(isEenDelayTimeSupported);
	EenDelayTimeUnitLabel->setVisible(isEenDelayTimeSupported);
}


void CExposureParamsGuiComp::OnGuiModelDetached()
{
	QObject::disconnect(ShutterTimeSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(DelayTimeSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(EenDelayTimeSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	ShutterTimeLabel->setVisible(false);
	ShutterTimeSB->setVisible(false);
	ShutterTimeUnitLabel->setVisible(false);

	DelayTimeLabel->setVisible(false);
	DelayTimeSB->setVisible(false);
	DelayTimeUnitLabel->setVisible(false);

	EenDelayTimeLabel->setVisible(false);
	EenDelayTimeSB->setVisible(false);
	EenDelayTimeUnitLabel->setVisible(false);

	BaseClass::OnGuiModelDetached();
}


// reimplemented (imod::IModelEditor)

void CExposureParamsGuiComp::UpdateModel() const
{
	icam::IExposureParams* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		UpdateBlocker blocker(const_cast<CExposureParamsGuiComp*>(this));

		double tolerance = 0.9e-6;

		istd::CChangeNotifier notifier(NULL);

		double shutterTime = ShutterTimeSB->value() * 0.001;
		if (ShutterTimeSB->isVisible() && (fabs(objectPtr->GetShutterTime() - shutterTime) > tolerance)){
			notifier.SetPtr(objectPtr);
			objectPtr->SetShutterTime(shutterTime);
		}

		double delayTime = DelayTimeSB->value() * 0.001;
		if (DelayTimeSB->isVisible() && (fabs(objectPtr->GetDelayTime() - delayTime) > tolerance)){
			notifier.SetPtr(objectPtr);
			objectPtr->SetDelayTime(delayTime);
		}

		double eenDelayTime = EenDelayTimeSB->value() * 0.001;
		if (EenDelayTimeSB->isVisible() && (fabs(objectPtr->GetEenDelay() - eenDelayTime) > tolerance)){
			notifier.SetPtr(objectPtr);
			objectPtr->SetEenDelay(eenDelayTime);
		}
	}
}


void CExposureParamsGuiComp::UpdateEditor()
{
	icam::IExposureParams* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		ShutterTimeSB->setValue(objectPtr->GetShutterTime() * 1000.0);
		DelayTimeSB->setValue(objectPtr->GetDelayTime() * 1000.0);
		EenDelayTimeSB->setValue(objectPtr->GetEenDelay() * 1000.0);
	}
}


// protected slots

void CExposureParamsGuiComp::OnParamsChanged(double /*value*/)
{
	UpdateModel();
}


} // namespace iqtcam


