#include "QtNemoPackage.h"



QtNemoPackage::QtNemoPackage()
	:acf::ComponentLibrary()
{
	acf::ComponentLibraryInfo info;
	info.setLibName("Qt Nemo");
	setLibraryInfo(info);
}


EXPORT_COMPONENT_LIBRARY(acf::QtNemoPackage);