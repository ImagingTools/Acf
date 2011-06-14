#ifndef iqt2d_CLine2dParamsGuiComp_included
#define iqt2d_CLine2dParamsGuiComp_included


#include "i2d/CLine2d.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqt2d/TSceneExtenderCompBase.h"
#include "iqt2d/Generated/ui_CLine2dParamsGuiComp.h"


namespace iqt2d
{


class CLine2dParamsGuiComp: public iqt2d::TSceneExtenderCompBase<iqtgui::TDesignerGuiObserverCompBase<
			Ui::CLine2dParamsGuiComp,
			i2d::CLine2d> >
{
	Q_OBJECT

public:
	typedef iqt2d::TSceneExtenderCompBase<iqtgui::TDesignerGuiObserverCompBase<
				Ui::CLine2dParamsGuiComp,
				i2d::CLine2d> > BaseClass;

	I_BEGIN_COMPONENT(CLine2dParamsGuiComp)
		I_ASSIGN(m_unitNameAttrPtr, "UnitName", "Name of geometric units e.g. mm", false, "mm");
		I_ASSIGN(m_lineZValueAttrPtr, "LineZValue", "Describe draw priority on display console (the objects with bigger value will overlap the other ones)", true, 1);
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
	I_ATTR(double, m_lineZValueAttrPtr);
};


} // namespace iqt2d


#endif // !iqt2d_CLine2dParamsGuiComp_included


