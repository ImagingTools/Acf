// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
// Qt includes
#include <QtCore/QCoreApplication>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif

// ACF includes
#include <ibase/IApplication.h>

#include <GeneratedFiles/ComposedGuiTest/CComposedGuiTest.h>


int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(iqtgui);
	Q_INIT_RESOURCE(AcfLoc);

	QApplication::addLibraryPath("./");

#ifdef Q_OS_WIN
	QApplication::setStyle("fusion");
#endif
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	CComposedGuiTest instance(NULL, true);

	ibase::IApplication* applicationPtr = instance.GetInterface<ibase::IApplication>();
	if (applicationPtr != NULL){
		applicationPtr->InitializeApplication(argc, argv);

		instance.EnsureAutoInitComponentsCreated();

		return applicationPtr->Execute(argc, argv);
	}

	return -1;
}


