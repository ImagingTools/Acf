// Qt includes
#include <QCoreApplication>

// ACF includes
#include "ibase/IApplication.h"
#include "iqt/CDefaultServicesProvider.h"

#include "Generated/CCompositor.h"


int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(iqtgui);
	Q_INIT_RESOURCE(icmpstr);

	iqt::CDefaultServicesProvider::RegisterServices();

	CCompositor instance;

	ibase::IApplication* applicationPtr = instance.GetInterface<ibase::IApplication>();

	if (applicationPtr != NULL){
		return applicationPtr->Execute(argc, argv);
	}

	return -1;
}

