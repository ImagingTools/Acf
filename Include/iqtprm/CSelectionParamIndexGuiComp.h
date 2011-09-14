#ifndef iqtprm_CSelectionParamIndexGuiComp_included
#define iqtprm_CSelectionParamIndexGuiComp_included


#include "iprm/ISelectionParam.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtprm/Generated/ui_CSelectionParamIndexGuiComp.h"


namespace iqtprm
{


class CSelectionParamIndexGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CSelectionParamIndexGuiComp,
			iprm::ISelectionParam>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CSelectionParamIndexGuiComp,
				iprm::ISelectionParam> BaseClass;

	I_BEGIN_COMPONENT(CSelectionParamIndexGuiComp);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

protected Q_SLOTS:
	void on_OptionIndexSlider_valueChanged(int value);
};


} // namespace iqtprm


#endif // !iqtprm_CSelectionParamIndexGuiComp_included


