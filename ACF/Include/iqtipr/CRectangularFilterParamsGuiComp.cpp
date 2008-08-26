#include "iqtipr/CRectangularFilterParamsGuiComp.h"


// Qt includes
#include <QString>

#include "istd/TChangeNotifier.h"


namespace iqtipr
{


// reimplemented (imod::IModelEditor)

void CRectangularFilterParamsGuiComp::UpdateModel() const
{
	if (!IsUpdateBlocked()){
		UpdateBlocker blocker(const_cast<CRectangularFilterParamsGuiComp*>(this));

		iipr::IMultidimensionalFilterParams* objectPtr = GetObjectPtr();
		if (objectPtr != NULL){
			imath::CVarVector filterLengths = objectPtr->GetFilterLengths();

			if (filterLengths.GetElementsCount() >= 1){
				filterLengths[0] = FilterWidthSlider->value() * 2 + 1;
			}

			if (filterLengths.GetElementsCount() >= 2){
				filterLengths[1] = FilterHeightSlider->value() * 2 + 1;
			}

			if (filterLengths != objectPtr->GetFilterLengths()){
				istd::CChangeNotifier notifier(objectPtr);

				objectPtr->SetFilterLengths(filterLengths);

				const_cast<CRectangularFilterParamsGuiComp&>(*this).UpdateLabel(filterLengths);
			}
		}
	}
}


void CRectangularFilterParamsGuiComp::UpdateEditor()
{
	iipr::IMultidimensionalFilterParams* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const imath::CVarVector& filterLengths = objectPtr->GetFilterLengths();

		UpdateLabel(filterLengths);

		int filterDimensionsCount = filterLengths.GetElementsCount();

		if (filterDimensionsCount >= 1){
			FilterWidthSlider->setValue(int(filterLengths[0] * 0.5 - 0.25));

			FilterWidthLabel->setVisible(true);
			FilterWidthSlider->setVisible(true);
			FilterWidthValueLabel->setVisible(true);
		}
		else{
			FilterWidthLabel->setVisible(false);
			FilterWidthSlider->setVisible(false);
			FilterWidthValueLabel->setVisible(false);
		}

		if (filterDimensionsCount >= 2){
			FilterHeightSlider->setValue(int(filterLengths[1] * 0.5 - 0.25));

			FilterHeightLabel->setVisible(true);
			FilterHeightSlider->setVisible(true);
			FilterHeightValueLabel->setVisible(true);
		}
		else{
			FilterHeightLabel->setVisible(false);
			FilterHeightSlider->setVisible(false);
			FilterHeightValueLabel->setVisible(false);
		}
	}
}


// protected methods

void CRectangularFilterParamsGuiComp::UpdateLabel(const imath::CVarVector& filterLengths)
{
	int filterDimensionsCount = filterLengths.GetElementsCount();

	if (filterDimensionsCount >= 1){
		FilterWidthValueLabel->setText(QString::number(int(filterLengths[0] + 0.5)));
	}

	if (filterDimensionsCount >= 2){
		FilterHeightValueLabel->setText(QString::number(int(filterLengths[1] + 0.5)));
	}
}


// protected slots

void CRectangularFilterParamsGuiComp::on_FilterWidthSlider_valueChanged(int /*value*/)
{
	UpdateModel();
}


void CRectangularFilterParamsGuiComp::on_FilterHeightSlider_valueChanged(int /*value*/)
{
	UpdateModel();
}


} // namespace iqtipr

