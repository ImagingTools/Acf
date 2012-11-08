#ifndef iqt2d_CPolygonParamsGuiComp_included
#define iqt2d_CPolygonParamsGuiComp_included


// ACF includes
#include "iview/CInteractivePolygonShape.h"

#include "iqt2d/TPolygonBasedParamsGuiComp.h"


namespace iqt2d
{


class CPolygonParamsGuiComp: public TPolygonBasedParamsGuiComp<iview::CInteractivePolygonShape, i2d::CPolygon>
{
	Q_OBJECT

public:
	typedef TPolygonBasedParamsGuiComp<iview::CInteractivePolygonShape, i2d::CPolygon> BaseClass;

	I_BEGIN_COMPONENT(CPolygonParamsGuiComp);
	I_END_COMPONENT;

protected Q_SLOTS:
	void OnParamsChanged();

	void on_InsertButton_clicked();
	void on_RemoveButton_clicked();
	void on_CopyButton_clicked();
	void on_PasteButton_clicked();

	void OnToolsButtonMenuActionTriggered(QAction* action);
};


} // namespace iqt2d


#endif // !iqt2d_CPolygonParamsGuiComp_included
