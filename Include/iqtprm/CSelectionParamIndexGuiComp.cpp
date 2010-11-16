#include "iqtprm/CSelectionParamIndexGuiComp.h"


// Qt includes
#include <QFileDialog>

// ACF includes
#include "istd/TChangeNotifier.h"

#include "iprm/ISelectionConstraints.h"


namespace iqtprm
{


// reimplemented (imod::IModelEditor)

void CSelectionParamIndexGuiComp::UpdateModel() const
{
	if (!IsUpdateBlocked()){
		UpdateBlocker blocker(const_cast<CSelectionParamIndexGuiComp*>(this));

		iprm::ISelectionParam* objectPtr = GetObjectPtr();
		if (objectPtr != NULL){
			int iterationCount = OptionIndexSpin->value();

			if (iterationCount != objectPtr->GetSelectedOptionIndex()){
				istd::CChangeNotifier notifier(objectPtr);

				objectPtr->SetSelectedOptionIndex(iterationCount);
			}
		}
	}
}


void CSelectionParamIndexGuiComp::UpdateEditor(int /*updateFlags*/)
{
	const iprm::ISelectionParam* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int maxCount = 10;
		const iprm::ISelectionConstraints* constraintsPtr = objectPtr->GetConstraints();
		if (constraintsPtr != NULL){
			maxCount = constraintsPtr->GetOptionsCount() - 1;
		}

		OptionIndexSpin->setMaximum(maxCount);
		OptionIndexSlider->setMaximum(maxCount);

		OptionIndexSpin->setValue(objectPtr->GetSelectedOptionIndex());
	}
}


// protected slots

void CSelectionParamIndexGuiComp::on_OptionIndexSlider_valueChanged(int /*value*/)
{
	UpdateModel();
}


} // namespace iqtprm

