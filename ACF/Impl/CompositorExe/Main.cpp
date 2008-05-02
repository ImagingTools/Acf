#include <QApplication>


#include "icomp/TSimComponentWrap.h"

#include "QtPck/QtPck.h"
#include "BasePck/BasePck.h"


int main(int argc, char *argv[])
{
	icomp::TSimComponentWrap<BasePck::ApplicationInfo> applicationInfo;
	applicationInfo.InitComponent();

	icomp::TSimComponentWrap<QtPck::GuiApplication> application;
	application.SetRef("ApplicationInfo", &applicationInfo);
	application.EnsureInitialized(argc, argv);

	icomp::TSimComponentWrap<QtPck::SplashScreen> splashScreenGui;
	splashScreenGui.SetStringAttr("ImagePath", "../../../Docs/Images/CompositorSplashScreen.png");
	splashScreenGui.SetStringAttr("ProductType", "ACF Tool");
	splashScreenGui.SetStringAttr("CopyrightText", "This is a part of ACF project.\nSee 'licence.txt' for copyright informations");
	splashScreenGui.SetRef("ApplicationInfo", &applicationInfo);
	splashScreenGui.InitComponent();

	icomp::TSimComponentWrap<QtPck::Log> logGui;
	logGui.InitComponent();

	application.SetRef("SplashScreen", &splashScreenGui);
	application.SetRef("MainGui", &logGui);
	application.InitComponent();

	return application.Execute(argc, argv);
}
