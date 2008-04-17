#include <QApplication>


#include "icomp/TSimComponentWrap.h"

#include "iqt/CApplicationComp.h"
#include "iqt/CSplashScreenGuiComp.h"
#include "iqt/CLogGuiComp.h"


int main(int argc, char *argv[])
{
	icomp::TSimComponentWrap<iqt::CApplicationComp> application;
	application.EnsureInitialized(argc, argv);

	icomp::TSimComponentWrap<iqt::CSplashScreenGuiComp> splashScreenComp;
	splashScreenGui.SetStringAttr("ImagePath", "../../../Docs/Images/AcfSplashScreen.png");
	splashScreenGui.SetStringAttr("ProductName", "Compositior");
	splashScreenGui.SetStringAttr("ProductType", "ACF Tool");
	splashScreenGui.SetStringAttr("CopyrightText", "See 'licence.txt' for copyright informations");
	splashScreenComp.InitComponent();

	icomp::TSimComponentWrap<iqt::CLogGuiComp> logGui;
	logGui.InitComponent();

	application.SetRef("SplashScreen", &splashScreenComp);
	application.SetRef("MainGui", &logGui);
	application.InitComponent();

	return application.Execute(argc, argv);
}
