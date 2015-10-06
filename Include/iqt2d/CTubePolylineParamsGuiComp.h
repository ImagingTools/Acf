#ifndef iqt2d_CTubePolylineParamsGuiComp_included
#define iqt2d_CTubePolylineParamsGuiComp_included


// ACF includes
#include "i2d/CTubePolyline.h"

#include "iview/CTubePolylineShape.h"

#include "iqt2d/TPolygonBasedParamsGuiComp.h"


namespace iqt2d
{


class CTubePolylineParamsGuiComp: public TPolygonBasedParamsGuiComp<iview::CTubePolylineShape, i2d::CTubePolyline>
{
	Q_OBJECT

public:
	typedef TPolygonBasedParamsGuiComp<iview::CTubePolylineShape, i2d::CTubePolyline> BaseClass;

	I_BEGIN_COMPONENT(CTubePolylineParamsGuiComp);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected Q_SLOTS:
	void OnParamsChanged();
	void OnActionTriggered(QAction* actionPtr);

	void on_InsertButton_clicked();
	void on_RemoveButton_clicked();
	void on_CopyButton_clicked();
	void on_PasteButton_clicked();

protected:
	// reimplemented (iqt2d::TPolygonBasedParamsGuiComp)
	virtual void OnInsertNode();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiRetranslate();
};


} // namespace iqt2d


#endif // !iqt2d_CTubePolylineParamsGuiComp_included
