#include "iqt2d/CRectangleParamsGuiComp.h"


#include "istd/TChangeNotifier.h"

#include "iqt2d/CRectangleShape.h"


namespace iqt2d
{


// reimplemented (iqt::TGuiObserverWrap)

void CRectangleParamsGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	QObject::connect(LeftSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(RightSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(BottomSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(TopSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
}


void CRectangleParamsGuiComp::OnGuiModelDetached()
{
	QObject::disconnect(LeftSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(RightSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(BottomSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(TopSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	BaseClass::OnGuiModelDetached();
}


// reimplemented (imod::IModelEditor)

void CRectangleParamsGuiComp::UpdateModel() const
{
	i2d::CRectangle* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		UpdateBlocker blocker(const_cast<CRectangleParamsGuiComp*>(this));

		istd::CChangeNotifier notifier(NULL);

		if (objectPtr->GetLeft() != LeftSpin->value()){
			notifier.SetPtr(objectPtr);
			objectPtr->SetLeft(LeftSpin->value());
		}

		if (objectPtr->GetRight() != RightSpin->value()){
			notifier.SetPtr(objectPtr);
			objectPtr->SetRight(RightSpin->value());
		}

		if (objectPtr->GetTop() != TopSpin->value()){
			notifier.SetPtr(objectPtr);
			objectPtr->SetTop(TopSpin->value());
		}

		if (objectPtr->GetBottom() != BottomSpin->value()){
			notifier.SetPtr(objectPtr);
			objectPtr->SetBottom(BottomSpin->value());
		}
	}
}


void CRectangleParamsGuiComp::UpdateEditor()
{
	i2d::CRectangle* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		LeftSpin->setValue(objectPtr->GetLeft());
		RightSpin->setValue(objectPtr->GetRight());
		BottomSpin->setValue(objectPtr->GetBottom());
		TopSpin->setValue(objectPtr->GetTop());
	}
}


// reimplemented (iqt2d::TSceneExtenderCompBase)

void CRectangleParamsGuiComp::CreateShapes(int /*sceneId*/, bool /*inactiveOnly*/, Shapes& result)
{
	imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		istd::TDelPtr<CRectangleShape> shapePtr(new CRectangleShape());
		if (shapePtr.IsValid()){
			if (modelPtr->AttachObserver(shapePtr.GetPtr())){
				result.PushBack(shapePtr.PopPtr());
			}
		}
	}
}


// protected slots

void CRectangleParamsGuiComp::OnParamsChanged(double /*value*/)
{
	UpdateModel();
}


} // namespace iqt2d

