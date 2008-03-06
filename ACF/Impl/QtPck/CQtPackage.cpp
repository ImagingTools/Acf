#ifdef OLD_ACF_SUPPORT

#include "CQtPackage.h"

#include "iqt/CTabContainerGuiComp.h"
#include "iqt/CLoginGuiComp.h"


CQtPackage::CQtPackage()
:	BaseClass()
{
	acf::ComponentLibraryInfo info;
	info.SetLibName("Qt");
	SetLibraryInfo(info);

	RegisterComponent<iqt::CTabContainerGuiComp>("TabContainerGui");
	RegisterComponent<iqt::CLoginGuiComp>("LoginGui");
}


EXPORT_COMPONENT_LIBRARY(CQtPackage);

#endif // OLD_ACF_SUPPORT
