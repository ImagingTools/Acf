#include <QApplication>

#include "icomp/TSimComponentWrap.h"

#include "iqt/CLoginGuiComp.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setStyle("plastique");

	QWidget mainWidget;

	icomp::TSimComponentWrap<iqt::CLoginGuiComp> loginGui;
	loginGui.CreateGui(&mainWidget);

	mainWidget.show();
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));

	return a.exec();
}


