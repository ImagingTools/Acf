#include "CQtDbPackage.h"

#include "iqtdb/CDatabaseConnectorComp.h"


CQtDbPackage::CQtDbPackage()
	:acf::ComponentLibrary()
{
	using namespace iqtdb;

	acf::ComponentLibraryInfo info;
	info.setLibName("Qt Database");
	SetLibraryInfo(info);

	RegisterComponent<CDatabaseConnectorComp>("DatabaseConnector");
}


EXPORT_COMPONENT_LIBRARY(CQtDbPackage);