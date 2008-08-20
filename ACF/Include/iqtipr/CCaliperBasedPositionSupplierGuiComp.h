#ifndef iqtipr_CCaliperBasedPositionSupplierGuiComp_included
#define iqtipr_CCaliperBasedPositionSupplierGuiComp_included


#include "iser/IFileLoader.h"

#include "imod/IObserver.h"
#include "imod/TModelWrap.h"

#include "i2d/CPosition2d.h"

#include "iproc/IIdManager.h"

#include "iipr/CCaliperBasedPositionSupplierComp.h"

#include "iqt/IGuiObject.h"
#include "iqt/TDesignerGuiObserverCompBase.h"

#include "iqtproc/TSupplierGuiCompBase.h"

#include "iqtipr/iqtipr.h"

#include "iqtipr/Generated/ui_CCaliperBasedPositionSupplierGuiComp.h"


namespace iqtipr
{


class CCaliperBasedPositionSupplierGuiComp: public iqtproc::TSupplierGuiCompBase<
			Ui::CCaliperBasedPositionSupplierGuiComp,
			iipr::CCaliperBasedPositionSupplierComp>
{
	Q_OBJECT

public:
	typedef iqtproc::TSupplierGuiCompBase<
				Ui::CCaliperBasedPositionSupplierGuiComp,
				iipr::CCaliperBasedPositionSupplierComp> BaseClass;

	I_BEGIN_COMPONENT(CCaliperBasedPositionSupplierGuiComp)
	I_END_COMPONENT

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor();

protected slots:
	void on_TestButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();

protected:
	// reimplemented (iqtproc::TSupplierGuiCompBase)
	virtual QWidget* GetParamsWidget() const;

	// reimplemented (iqt2d::TSceneExtenderCompBase)
	virtual void CreateShapes(int sceneId, bool inactiveOnly, Shapes& result);

	// reimplemented (iqt::TGuiObserverWrap)
	virtual void OnGuiModelAttached();

private:
	imod::TModelWrap<i2d::CPosition2d> m_foundPosition;
};


} // namespace iqtipr


#endif // !iqtipr_CCaliperBasedPositionSupplierGuiComp_included


