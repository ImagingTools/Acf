#ifndef iqt_CLoginGuiComp_included
#define iqt_CLoginGuiComp_included


#include "iqt/TDesignerGuiComponentBase.h"

#include "iqt/GeneratedFiles/ui_CLoginGuiComp.h"


namespace iqt
{


class CLoginGuiComp: public TDesignerGuiComponentBase<Ui::CLoginGuiComp, QWidget>
{
public:
	typedef TDesignerGuiComponentBase<Ui::CLoginGuiComp, QWidget> BaseClass;
	I_COMPONENT(CLoginGuiComp);

	CLoginGuiComp(const icomp::IComponentContext* contextPtr);

protected:
	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiCreated();
};


} // namespace iqt


#endif // !iqt_CLoginGuiComp_included


