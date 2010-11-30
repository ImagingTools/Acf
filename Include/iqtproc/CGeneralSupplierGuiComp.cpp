#include "iqtproc/CGeneralSupplierGuiComp.h"


namespace iqtproc
{


// reimplemented (imod::IModelEditor)

void CGeneralSupplierGuiComp::UpdateModel() const
{
}


void CGeneralSupplierGuiComp::UpdateEditor(int /*updateFlags*/)
{
	if (IsGuiCreated()){
		iproc::ISupplier* supplierPtr = GetObjectPtr();
		if (supplierPtr != NULL){
			double inspectionTime = supplierPtr->GetWorkDurationTime();
			QString inspectionText = tr("%1 ms").arg(inspectionTime * 1000);
			InspectionTimeLabel->setText(inspectionText);
		}
		else{
			InspectionTimeLabel->setText(tr("-"));
		}
	}
}


// protected slots

void CGeneralSupplierGuiComp::on_TestButton_clicked()
{
	iproc::ISupplier* supplierPtr = GetObjectPtr();
	if (supplierPtr != NULL){
		supplierPtr->InvalidateSupplier();
		supplierPtr->EnsureWorkFinished();
	}
}


void CGeneralSupplierGuiComp::on_LoadParamsButton_clicked()
{
	LoadParams();
}


void CGeneralSupplierGuiComp::on_SaveParamsButton_clicked()
{
	SaveParams();
}


// protected methods

// reimplemented (iqtproc::TSupplierGuiCompBase)

QWidget* CGeneralSupplierGuiComp::GetParamsWidget() const
{
	I_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CGeneralSupplierGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	ParamsGB->setVisible(AreParamsEditable() || IsLoadParamsSupported());

	LoadParamsButton->setVisible(IsLoadParamsSupported());
	SaveParamsButton->setVisible(IsSaveParamsSupported());
}


} // namespace iqtproc


