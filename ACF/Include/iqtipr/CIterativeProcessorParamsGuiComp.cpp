#include "iqtipr/CIterativeProcessorParamsGuiComp.h"


// Qt includes
#include <QFileDialog>

// ACF includes
#include "istd/TChangeNotifier.h"


namespace iqtipr
{


// reimplemented (imod::IModelEditor)

void CIterativeProcessorParamsGuiComp::UpdateModel() const
{
	if (!IsUpdateBlocked()){
		UpdateBlocker blocker(const_cast<CIterativeProcessorParamsGuiComp*>(this));

		iipr::CIterativeProcessorParams* objectPtr = GetObjectPtr();
		if (objectPtr != NULL){
			int iterationCount = IterationsSpin->value();

			if (iterationCount != objectPtr->GetIterationsCount()){
				istd::CChangeNotifier notifier(objectPtr);

				objectPtr->SetIterationsCount(iterationCount);
			}
		}
	}
}


void CIterativeProcessorParamsGuiComp::UpdateEditor()
{
	iipr::CIterativeProcessorParams* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		IterationsSpin->setValue(objectPtr->GetIterationsCount());
	}
}


// protected slots

void CIterativeProcessorParamsGuiComp::on_IterationsSlider_valueChanged(int /*value*/)
{
	UpdateModel();
}


} // namespace iqtipr

