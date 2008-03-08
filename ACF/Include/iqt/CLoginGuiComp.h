#ifndef iqt_CLoginGuiComp_included
#define iqt_CLoginGuiComp_included


#include "iqt/TGuiComponentBase.h"

#include "iqt/GeneratedFiles/ui_CLoginGuiComp.h"


namespace iqt
{


class CLoginGuiComp: public TGuiComponentBase<Ui::CLoginGuiComp, QWidget>
{
public:
	typedef TGuiComponentBase<Ui::CLoginGuiComp, QWidget> BaseClass;
	I_COMPONENT(CLoginGuiComp);

	CLoginGuiComp(const icomp::IComponentContext* contextPtr);

protected:
	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiCreated();
};


} // namespace iqt


#endif // !iqt_CLoginGuiComp_included


