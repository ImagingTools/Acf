#include <QGridLayout>

#include "iqt/CLoginGuiComp.h"


namespace iqt
{


CLoginGuiComp::CLoginGuiComp(const icomp::IComponentContext* contextPtr)
:	BaseClass(contextPtr)
{
}


// protected methods

// reimplemented (iqt::CGuiComponentBase)

void CLoginGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();
}


} // namespace iqt


