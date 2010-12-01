#include "iqtgui/CImagePropertyGuiComp.h"


namespace iqtgui
{


// public methods

// reimplemented (imod::IModelEditor)

void CImagePropertyGuiComp::UpdateEditor(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	PropertyTree->clear();

	iimg::IBitmap* bitmapPtr = GetObjectPtr();
	if (bitmapPtr != NULL && IsGuiCreated()){
		QTreeWidgetItem* widthItemPtr = new QTreeWidgetItem();
		widthItemPtr->setText(0, "Width");
		widthItemPtr->setText(1, QString("%1").arg(bitmapPtr->GetImageSize().GetX()));

		QTreeWidgetItem* heightItemPtr = new QTreeWidgetItem();
		heightItemPtr->setText(0, "Height");
		heightItemPtr->setText(1, QString("%1").arg(bitmapPtr->GetImageSize().GetY()));

		PropertyTree->addTopLevelItem(widthItemPtr);
		PropertyTree->addTopLevelItem(heightItemPtr);
	}
}


void CImagePropertyGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated() && (GetObjectPtr() != NULL));
}


} // namespace iqtgui


