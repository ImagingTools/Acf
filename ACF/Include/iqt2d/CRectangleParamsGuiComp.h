#ifndef iqt2d_CRectangleParamsGuiComp_included
#define iqt2d_CRectangleParamsGuiComp_included


#include "i2d/CRectangle.h"

#include "iqt/TDesignerGuiObserverCompBase.h"

#include "iqt2d/TSceneExtenderCompBase.h"

#include "iqt2d/Generated/ui_CRectangleParamsGuiComp.h"


namespace iqt2d
{


class CRectangleParamsGuiComp: public iqt2d::TSceneExtenderCompBase<iqt::TDesignerGuiObserverCompBase<
			Ui::CRectangleParamsGuiComp,
			i2d::CRectangle> >
{
	Q_OBJECT

public:
	typedef iqt2d::TSceneExtenderCompBase<iqt::TDesignerGuiObserverCompBase<
				Ui::CRectangleParamsGuiComp,
				i2d::CRectangle> > BaseClass;

	I_BEGIN_COMPONENT(CRectangleParamsGuiComp);
	I_END_COMPONENT;

	// reimplemented (iqt::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor();

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);

	// reimplemented (iqt2d::TSceneExtenderCompBase)
	virtual void CreateShapes(int sceneId, bool inactiveOnly, Shapes& result);

protected slots:
	void OnParamsChanged(double value);
};


} // namespace iqt2d


#endif // !iqt2d_CRectangleParamsGuiComp_included


