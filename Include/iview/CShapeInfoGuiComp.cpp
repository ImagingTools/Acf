#include "iview/CShapeInfoGuiComp.h"


namespace iview
{


// protected methods

void CShapeInfoGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	IShapeStatusInfo* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		i2d::CVector2d logicalPosition = objectPtr->GetLogicalPosition();
		LogicalPosition->setText(tr("X: %1  Y: %2").arg(logicalPosition.GetX()).arg(logicalPosition.GetY()));

		i2d::CVector2d pixelPosition = objectPtr->GetPixelPosition();
		PixelPosition->setText(tr("X: %1  Y: %2").arg(pixelPosition.GetX()).arg(pixelPosition.GetY()));

		QString infoText = objectPtr->GetInfoText();
		InfoText->setText(infoText);
	}
}


} // namespace iview


