#include <QApplication>


#include "icomp/TSimComponentWrap.h"

#include "QtPck/QtPck.h"


int main(int argc, char *argv[])
{
	icomp::TSimComponentWrap<QtPck::GuiApplication> application;
	application.EnsureInitialized(argc, argv);

	icomp::TSimComponentWrap<QtPck::SplashScreen> splashScreenGui;
	splashScreenGui.SetStringAttr("ImagePath", "../../../Docs/Images/AcfSplashScreen.png");
	splashScreenGui.SetStringAttr("ProductName", "Compositor");
	splashScreenGui.SetStringAttr("ProductType", "ACF Tool");
	splashScreenGui.SetStringAttr("CopyrightText", "This is a part of ACF project.\nSee 'licence.txt' for copyright informations");
	splashScreenGui.InitComponent();

	icomp::TSimComponentWrap<QtPck::Log> logGui;
	logGui.InitComponent();

	application.SetRef("SplashScreen", &splashScreenGui);
	application.SetRef("MainGui", &logGui);
	application.InitComponent();

	return application.Execute(argc, argv);
}
