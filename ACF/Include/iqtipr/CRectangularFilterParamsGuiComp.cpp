#include "iqtipr/CRectangularFilterParamsGuiComp.h"


// Qt includes
#include <QFileDialog>

// ACF includes
#include "istd/TChangeNotifier.h"


namespace iqtipr
{


// reimplemented (imod::IModelEditor)

void CRectangularFilterParamsGuiComp::UpdateModel() const
{
	if (!IsUpdateBlocked()){
		UpdateBlocker blocker(const_cast<CRectangularFilterParamsGuiComp*>(this));

		iipr::CRectangularFilterParams* objectPtr = GetObjectPtr();
		if (objectPtr != NULL){
			int filterWidth = FilterWidthSpin->value();
			int filterHeight = FilterHeightSpin->value();

			ibase::CSize filterSize(filterWidth, filterHeight);

			if (filterSize != objectPtr->GetSize()){
				istd::CChangeNotifier notifier(objectPtr);

				objectPtr->SetSize(filterSize);
			}
		}
	}
}


void CRectangularFilterParamsGuiComp::UpdateEditor()
{
	iipr::CRectangularFilterParams* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const ibase::CSize& filterSize = objectPtr->GetSize();

		FilterWidthSpin->setValue(filterSize.GetX());
		FilterHeightSpin->setValue(filterSize.GetY());
	}
}


// protected slots

void CRectangularFilterParamsGuiComp::on_FilterHeightSlider_valueChanged(int /*value*/)
{
	UpdateModel();
}


void CRectangularFilterParamsGuiComp::on_FilterWidthSlider_valueChanged(int /*value*/)
{
	UpdateModel();
}


} // namespace iqtipr

