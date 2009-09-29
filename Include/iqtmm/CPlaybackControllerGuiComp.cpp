#include "iqtmm/CPlaybackControllerGuiComp.h"


// Qt includes
#include <QCoreApplication>
#include <QEventLoop>
#include <QUrl>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"




namespace iqtmm
{


// public methods
	
// reimplemented (imod::IModelEditor)
	
void CPlaybackControllerGuiComp::UpdateEditor(int updateFlags)
{
	imm::IMediaController* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		if ((updateFlags & imm::IMediaController::CF_STATUS) != 0){
			iqt::CSignalBlocker block(this, true);

			PlayButton->setChecked(objectPtr->IsPlaying());
		}

		if ((updateFlags & imm::IMediaController::CF_POSITION) != 0){
			iqt::CSignalBlocker block(this, true);

			PositionSlider->setValue(objectPtr->GetCurrentPosition() * 100);
		}
	}
}


void CPlaybackControllerGuiComp::UpdateModel() const
{
}


// private slots

void CPlaybackControllerGuiComp::on_PlayButton_toggled(bool isToggled)
{
	imm::IMediaController* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		UpdateBlocker block(this);

		objectPtr->SetPlaying(isToggled);
	}
}


void CPlaybackControllerGuiComp::on_PositionSlider_valueChanged(int position)
{
	imm::IMediaController* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		UpdateBlocker block(this);

		objectPtr->SetCurrentPosition(position / 100.0);
	}
}



} // namespace iqtmm


