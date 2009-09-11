#include "iqtmm/CVideoWidgetGuiComp.h"


namespace iqtmm
{


// public methods


// reimplemented (imod::IModelEditor)

void CVideoWidgetGuiComp::UpdateEditor(int/* updateFlags*/)
{
}


void CVideoWidgetGuiComp::UpdateModel() const
{
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CVideoWidgetGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	Phonon::MediaObject* mediaObjectPtr = dynamic_cast<Phonon::MediaObject*>(GetObjectPtr());

	if (mediaObjectPtr != NULL){
		Phonon::createPath(mediaObjectPtr, GetQtWidget());

		mediaObjectPtr->play();
	}
}


void CVideoWidgetGuiComp::OnGuiModelDetached()
{
	Phonon::MediaObject* mediaObjectPtr = dynamic_cast<Phonon::MediaObject*>(GetObjectPtr());

	if (mediaObjectPtr != NULL){
		mediaObjectPtr->stop();
	}

	BaseClass::OnGuiModelDetached();

}


} // namespace iqtmm


