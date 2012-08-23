#include "iqt2d/CPolygonParamsGuiComp.h"


namespace iqt2d
{


// protected slots

void CPolygonParamsGuiComp::OnParamsChanged()
{
	DoUpdateModel();
}


void CPolygonParamsGuiComp::on_InsertButton_clicked()
{
	OnInsertNode();
}


void CPolygonParamsGuiComp::on_RemoveButton_clicked()
{
	OnRemoveNode();
}


} // namespace iqt2d
