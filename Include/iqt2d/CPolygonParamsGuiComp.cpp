// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iqt2d/CPolygonParamsGuiComp.h>


namespace iqt2d
{


CPolygonParamsGuiComp::CPolygonParamsGuiComp() 
{
}


// protected slots

void CPolygonParamsGuiComp::OnParamsChanged()
{
	DoUpdateModel();
}


void CPolygonParamsGuiComp::OnActionTriggered(QAction* actionPtr)
{
	BaseClass::OnActionTriggered(actionPtr);
}


void CPolygonParamsGuiComp::on_InsertButton_clicked()
{
	OnInsertNode();
}


void CPolygonParamsGuiComp::on_RemoveButton_clicked()
{
	auto polygonPtr = GetObservedObject();
	Q_ASSERT(polygonPtr != nullptr);

	if (polygonPtr == nullptr){
		return;
	}

	OnRemoveNode();
}


void CPolygonParamsGuiComp::on_CopyButton_clicked()
{
	OnCopyData();
}


void CPolygonParamsGuiComp::on_PasteButton_clicked()
{
	OnPasteData();
}


} // namespace iqt2d
