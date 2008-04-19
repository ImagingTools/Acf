#include <QApplication>


#include "icomp/TSimComponentWrap.h"

#include "iqt/CApplicationComp.h"
#include "iqt/CSplashScreenGuiComp.h"
#include "iqt/CLogGuiComp.h"


int main(int argc, char *argv[])
{
	icomp::TSimComponentWrap<iqt::CApplicationComp> application;
	application.EnsureInitialized(argc, argv);

	icomp::TSimComponentWrap<iqt::CSplashScreenGuiComp> splashScreenGui;
	splashScreenGui.SetStringAttr("ImagePath", "../../../Docs/Images/AcfSplashScreen.png");
	splashScreenGui.SetStringAttr("ProductName", "Compositor");
	splashScreenGui.SetStringAttr("ProductType", "ACF Tool");
	splashScreenGui.SetStringAttr("CopyrightText", "This is a part of ACF project.\nSee 'licence.txt' for copyright informations");
	splashScreenGui.InitComponent();

	icomp::TSimComponentWrap<iqt::CLogGuiComp> logGui;
	logGui.InitComponent();

	application.SetRef("SplashScreen", &splashScreenGui);
	application.SetRef("MainGui", &logGui);
	application.InitComponent();

	return application.Execute(argc, argv);
}
