#include "iqtproc/CIterativeProcessorParamsGuiComp.h"


// Qt includes
#include <QFileDialog>

// ACF includes
#include "istd/TChangeNotifier.h"


namespace iqtproc
{


// reimplemented (imod::IModelEditor)

void CIterativeProcessorParamsGuiComp::UpdateModel() const
{
	if (!IsUpdateBlocked()){
		UpdateBlocker blocker(const_cast<CIterativeProcessorParamsGuiComp*>(this));

		iproc::CIterativeProcessorParams* objectPtr = GetObjectPtr();
		if (objectPtr != NULL){
			int iterationCount = IterationsSpin->value();

			if (iterationCount != objectPtr->GetIterationsCount()){
				istd::CChangeNotifier notifier(objectPtr);

				objectPtr->SetIterationsCount(iterationCount);
			}
		}
	}
}


void CIterativeProcessorParamsGuiComp::UpdateEditor(int /*updateFlags*/)
{
	iproc::CIterativeProcessorParams* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		IterationsSpin->setValue(objectPtr->GetIterationsCount());
	}
}


// protected slots

void CIterativeProcessorParamsGuiComp::on_IterationsSlider_valueChanged(int /*value*/)
{
	UpdateModel();
}


} // namespace iqtproc

