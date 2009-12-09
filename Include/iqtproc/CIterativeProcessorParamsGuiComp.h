#ifndef iqt_CIterativeProcessorParamsGuiComp_included
#define iqt_CIterativeProcessorParamsGuiComp_included


#include "iproc/CIterativeProcessorParams.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtproc/Generated/ui_CIterativeProcessorParamsGuiComp.h"


namespace iqtproc
{


class CIterativeProcessorParamsGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CIterativeProcessorParamsGuiComp,
			iproc::CIterativeProcessorParams>
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CIterativeProcessorParamsGuiComp,
				iproc::CIterativeProcessorParams> BaseClass;

	I_BEGIN_COMPONENT(CIterativeProcessorParamsGuiComp)
	I_END_COMPONENT

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

protected slots:
	void on_IterationsSlider_valueChanged(int value);
};


} // namespace iqtproc


#endif // !iqt_CIterativeProcessorParamsGuiComp_included


