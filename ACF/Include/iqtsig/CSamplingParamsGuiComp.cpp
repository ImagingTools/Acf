#include "iqtsig/CSamplingParamsGuiComp.h"


#include "istd/TChangeNotifier.h"

#include "isig/ISamplingConstraints.h"


namespace iqtsig
{


// reimplemented (imod::IModelEditor)

void CSamplingParamsGuiComp::UpdateModel() const
{
}


void CSamplingParamsGuiComp::UpdateEditor()
{
	isig::ISamplingParams* objectPtr = GetObjectPtr();
	if (IsGuiCreated() && (objectPtr != NULL)){
		IntervalSB->setValue(objectPtr->GetInterval() * 1000);
		ModeCB->setCurrentIndex(objectPtr->GetSamplingMode());
	}
}


// reimplemented (iqt::TGuiObserverWrap)

void CSamplingParamsGuiComp::OnGuiModelAttached()
{
	I_ASSERT(IsGuiCreated());
	isig::ISamplingParams* objectPtr = GetObjectPtr();
	I_ASSERT(objectPtr != NULL);

	istd::CRange range(0, 0.1);

	const isig::ISamplingConstraints* constraintsPtr = objectPtr->GetConstraints();
	if (constraintsPtr != NULL){
		range = constraintsPtr->GetIntervalRange();
	}

	IntervalSB->setMinimum(range.GetMinValue() * 1000);
	IntervalSB->setMaximum(range.GetMaxValue() * 1000);
}


// protected slots

void CSamplingParamsGuiComp::on_IntervalSB_valueChanged(double value)
{
	isig::ISamplingParams* objectPtr = GetObjectPtr();
	if (IsGuiCreated() && (objectPtr != NULL)){
		double interval = value * 0.001;
		if (fabs(interval - objectPtr->GetInterval()) > I_EPSILON){
			istd::CChangeNotifier notifier(objectPtr);

			objectPtr->SetInterval(interval);
		}
	}
}


void CSamplingParamsGuiComp::on_ModeCB_currentIndexChanged(int index)
{
	isig::ISamplingParams* objectPtr = GetObjectPtr();
	if (!IsGuiCreated() || (objectPtr == NULL)){
		return;
	}

	if (index != objectPtr->GetSamplingMode()){
		const isig::ISamplingConstraints* constraintsPtr = objectPtr->GetConstraints();
		if (constraintsPtr != NULL){
			if (!constraintsPtr->IsSamplingModeSupported(index)){
				return;
			}
		}

		istd::CChangeNotifier notifier(objectPtr);

		objectPtr->SetSamplingMode(index);
	}
}


} // namespace iqtsig


