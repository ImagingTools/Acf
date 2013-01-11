#include "iqt2d/CAffineTransformationParamsGuiComp.h"

namespace iqt2d
{

	
// public methods

// reimplemented (imod::IModelEditor)

void CAffineTransformationParamsGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	i2d::CAffineTransformation2d* objectPtr = GetObjectPtr();
	Q_ASSERT(objectPtr != NULL);

	i2d::CVector2d translation(TranslationSpinX->value(), TranslationSpinY->value());

	istd::CChangeNotifier notifier(NULL);

	if(objectPtr->GetTransformation().GetTranslation() != translation){
		notifier.SetPtr(objectPtr);
		objectPtr->Reset(translation);		
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CAffineTransformationParamsGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	connect(TranslationSpinX, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	connect(TranslationSpinY, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));	
}


void CAffineTransformationParamsGuiComp::OnGuiModelDetached()
{
	disconnect(TranslationSpinX, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	disconnect(TranslationSpinY, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));	

	BaseClass::OnGuiModelDetached();
}


void CAffineTransformationParamsGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	i2d::CAffineTransformation2d* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		i2d::CVector2d translation = objectPtr->GetTransformation().GetTranslation();

		TranslationSpinX->setValue(translation.GetX());
		TranslationSpinY->setValue(translation.GetY());		

		UpdateAllViews();
	}
}


// protected slots

void CAffineTransformationParamsGuiComp::OnParamsChanged(double /*value*/)
{
	DoUpdateModel();
}

}