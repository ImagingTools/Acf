#include "iqtgui/CImagePropertiesFrameComp.h"


namespace iqtgui
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CImagePropertiesFrameComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	iimg::IBitmap* bitmapPtr = GetObjectPtr();
	if (bitmapPtr != NULL && IsGuiCreated()){
		SizeLabel->setText(QString("%1x%2").arg(bitmapPtr->GetImageSize().GetX()).arg(bitmapPtr->GetImageSize().GetY()));
		DepthLabel->setText(QString("%1 bits").arg(bitmapPtr->GetComponentBitsCount() * bitmapPtr->GetComponentsCount()));
	}
}


} // namespace iqtgui


