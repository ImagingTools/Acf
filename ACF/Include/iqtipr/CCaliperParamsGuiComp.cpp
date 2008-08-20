#include "iqtipr/CCaliperParamsGuiComp.h"


#include "istd/TChangeNotifier.h"


namespace iqtipr
{


// reimplemented (imod::IModelEditor)

void CCaliperParamsGuiComp::UpdateModel() const
{
	iipr::ICaliperParams* objectPtr = GetObjectPtr();
	if (objectPtr != NULL && !IsUpdateBlocked()){
		UpdateBlocker blocker(const_cast<CCaliperParamsGuiComp*>(this));

		istd::CChangeNotifier notifier(NULL);

		double threshold = ThresholdSB->value() * 0.01;
		if (fabs(objectPtr->GetWeightThreshold() - threshold) > I_BIG_EPSILON){
			notifier.SetPtr(objectPtr);
			objectPtr->SetWeightThreshold(threshold);
		}

		int edgePolarity = EdgePolarityCB->currentIndex();
		if (objectPtr->GetPolarityMode() != edgePolarity){
			notifier.SetPtr(objectPtr);
			objectPtr->SetPolarityMode(edgePolarity);
		}

		int direction = DirectionCB->currentIndex();
		if (objectPtr->GetDirectionMode() != direction){
			notifier.SetPtr(objectPtr);
			objectPtr->SetDirectionMode(direction);
		}
	}
}


void CCaliperParamsGuiComp::UpdateEditor()
{
	iipr::ICaliperParams* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		double threshold = objectPtr->GetWeightThreshold();
		ThresholdSB->setValue(int(threshold * 100 + 0.5));

		int edgePolarity = objectPtr->GetPolarityMode();
		EdgePolarityCB->setCurrentIndex(edgePolarity);

		int direction = objectPtr->GetDirectionMode();
		DirectionCB->setCurrentIndex(direction);
	}
}


// protected slots

void CCaliperParamsGuiComp::on_ThresholdSB_valueChanged(int /*value*/)
{
	UpdateModel();
}


void CCaliperParamsGuiComp::on_EdgePolarityCB_currentIndexChanged(int /*index*/)
{
	UpdateModel();
}


void CCaliperParamsGuiComp::on_DirectionCB_currentIndexChanged(int /*index*/)
{
	UpdateModel();
}


} // namespace iqtipr


