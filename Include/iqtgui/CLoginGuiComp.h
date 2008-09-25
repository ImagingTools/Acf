#ifndef iqtgui_CLoginGuiComp_included
#define iqtgui_CLoginGuiComp_included


#include "iqtgui/TDesignerGuiCompBase.h"

#include "iqtgui/Generated/ui_CLoginGuiComp.h"


namespace iqtgui
{


class CLoginGuiComp: public TDesignerGuiCompBase<Ui::CLoginGuiComp, QWidget>
{
public:
	typedef TDesignerGuiCompBase<Ui::CLoginGuiComp, QWidget> BaseClass;

	I_BEGIN_COMPONENT(CLoginGuiComp)
	I_END_COMPONENT

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
};


} // namespace iqtgui


#endif // !iqtgui_CLoginGuiComp_included

