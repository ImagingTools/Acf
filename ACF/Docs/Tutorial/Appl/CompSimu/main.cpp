#include <QApplication>

#include "icomp/TSimComponentWrap.h"

#include "iqt/CApplicationComp.h"
#include "iqt/CSplashScreenGuiComp.h"
#include "iqt/CLoginGuiComp.h"


int main(int argc, char *argv[])
{
	icomp::TSimComponentWrap<iqt::CApplicationComp> application;
	application.EnsureInitialized(argc, argv);

	icomp::TSimComponentWrap<iqt::CSplashScreenGuiComp> splashScreenGui;
	splashScreenGui.SetStringAttr("ImagePath", "../../../Docs/Images/AcfSplashScreen.png");
	splashScreenGui.SetStringAttr("ProductName", "CompSimu");
	splashScreenGui.SetStringAttr("ProductType", "Tutorial");
	splashScreenGui.SetStringAttr("CopyrightText", "This is a part of ACF project.\nSee 'licence.txt' for copyright informations");
	splashScreenGui.InitComponent();

	icomp::TSimComponentWrap<iqt::CLoginGuiComp> loginGui;
	loginGui.InitComponent();

	application.SetRef("MainGui", &loginGui);
	application.SetRef("SplashScreen", &splashScreenGui);
	application.InitComponent();

	return application.Execute(argc, argv);
}


