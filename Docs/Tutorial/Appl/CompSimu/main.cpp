// ACF includes
#include "icomp/TSimComponentWrap.h"

// ACF package includes
#include "QtGuiPck/QtGuiPck.h"


int main(int argc, char *argv[])
{
	icomp::TSimComponentWrap<QtGuiPck::GuiApplication> application;
	application.InitializeApplication(argc, argv);

	icomp::TSimComponentWrap<QtGuiPck::SplashScreen> splashScreenGui;
	splashScreenGui.SetStringAttr("ImagePath", "../../../Docs/Images/AcfSplashScreen.png");
	splashScreenGui.SetStringAttr("ProductName", "CompSimu");
	splashScreenGui.SetStringAttr("ProductType", "Tutorial");
	splashScreenGui.SetStringAttr("CopyrightText", "This is a part of ACF project.\nSee 'licence.txt' for copyright informations");
	splashScreenGui.InitComponent();

	icomp::TSimComponentWrap<QtGuiPck::LoginGui> loginGui;
	loginGui.InitComponent();

	application.SetRef("MainGui", &loginGui);
	application.SetRef("SplashScreen", &splashScreenGui);
	application.InitComponent();

	return application.Execute(argc, argv);
}


