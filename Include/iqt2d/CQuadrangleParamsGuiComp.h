#ifndef iqt2d_CQuadrangleParamsGuiComp_included
#define iqt2d_CQuadrangleParamsGuiComp_included


#include "i2d/CQuadrangle.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqt2d/TSceneExtenderCompBase.h"
#include "iqt2d/Generated/ui_CQuadrangleParamsGuiComp.h"


namespace iqt2d
{


class CQuadrangleParamsGuiComp: public iqt2d::TSceneExtenderCompBase<iqtgui::TDesignerGuiObserverCompBase<
			Ui::CQuadrangleParamsGuiComp,
			i2d::CQuadrangle> >
{
	Q_OBJECT

public:
	typedef iqt2d::TSceneExtenderCompBase<iqtgui::TDesignerGuiObserverCompBase<
				Ui::CQuadrangleParamsGuiComp,
				i2d::CQuadrangle> > BaseClass;

	I_BEGIN_COMPONENT(CQuadrangleParamsGuiComp)
		I_ASSIGN(m_unitNameAttrPtr, "UnitName", "Name of geometric units e.g. mm", false, "mm");
		I_ASSIGN(m_shapeZValueAttrPtr, "ShapeZValue", "Describe draw priority on display console (the objects with bigger value will overlap the other ones)", true, 1);
	I_END_COMPONENT

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);

	// reimplemented (iqt2d::TSceneExtenderCompBase)
	virtual void CreateShapes(int sceneId, bool inactiveOnly, Shapes& result);

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(int updateFlags = 0);

protected Q_SLOTS:
	void OnParamsChanged(double value);

private:
	I_ATTR(istd::CString, m_unitNameAttrPtr);
	I_ATTR(double, m_shapeZValueAttrPtr);
};


} // namespace iqt2d


#endif // !iqt2d_CQuadrangleParamsGuiComp_included


