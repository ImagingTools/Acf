#include "CQtPackage.h"

#include "iqt/CTabContainerGuiComp.h"


CQtPackage::CQtPackage()
	:BaseClass()
{
	acf::ComponentLibraryInfo info;
	info.setLibName("Qt");
	setLibraryInfo(info);

	registerComponent<iqt::CTabContainerGuiComp>("TabContainerGui");
}


EXPORT_COMPONENT_LIBRARY(CQtPackage);