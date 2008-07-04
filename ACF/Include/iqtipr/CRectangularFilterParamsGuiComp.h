#ifndef iqt_CRectangularFilterParamsGuiComp_included
#define iqt_CRectangularFilterParamsGuiComp_included


#include "iipr/CRectangularFilterParams.h"

#include "iqt/TDesignerGuiObserverCompBase.h"

#include "iqtipr/Generated/ui_CRectangularFilterParamsGuiComp.h"


namespace iqtipr
{


class CRectangularFilterParamsGuiComp: public iqt::TDesignerGuiObserverCompBase<
			Ui::CRectangularFilterParamsGuiComp,
			iipr::CRectangularFilterParams>
{
	Q_OBJECT

public:
	typedef iqt::TDesignerGuiObserverCompBase<
				Ui::CRectangularFilterParamsGuiComp,
				iipr::CRectangularFilterParams> BaseClass;

	I_BEGIN_COMPONENT(CRectangularFilterParamsGuiComp)
	I_END_COMPONENT

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor();

protected slots:
	void on_FilterHeightSlider_valueChanged(int value);
	void on_FilterWidthSlider_valueChanged(int value);
	void on_IterationsSlider_valueChanged(int value);
};


} // namespace iqtipr


#endif // !iqt_CRectangularFilterParamsGuiComp_included


