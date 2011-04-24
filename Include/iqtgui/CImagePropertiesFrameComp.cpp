#include "iqtgui/CImagePropertiesFrameComp.h"


namespace iqtgui
{


// public methods

// reimplemented (imod::IModelEditor)

void CImagePropertiesFrameComp::UpdateEditor(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	iimg::IBitmap* bitmapPtr = GetObjectPtr();
	if (bitmapPtr != NULL && IsGuiCreated()){
		SizeLabel->setText(QString("%1x%2").arg(bitmapPtr->GetImageSize().GetX()).arg(bitmapPtr->GetImageSize().GetY()));
		DepthLabel->setText(QString("%1 bits").arg(bitmapPtr->GetComponentBitsCount() * bitmapPtr->GetComponentsCount()));
	}
}


void CImagePropertiesFrameComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated() && (GetObjectPtr() != NULL));
}


} // namespace iqtgui


