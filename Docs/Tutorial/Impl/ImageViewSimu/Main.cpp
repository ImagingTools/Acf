// ACF includes
#include "icomp/TSimComponentWrap.h"
#include "icomp/TSimComponentsFactory.h"

// ACF package includes
#include "BasePck/BasePck.h"
#include "QtPck/QtPck.h"
#include "QtGuiPck/QtGuiPck.h"
#include "QtViewPck/QtViewPck.h"


int main(int argc, char *argv[])
{
	icomp::TSimComponentWrap<QtGuiPck::GuiApplication> application;
	application.InitializeApplication(argc, argv);

	Q_INIT_RESOURCE(iqtgui);

	icomp::TSimComponentWrap<BasePck::ApplicationInfo> applicationInfo;
	applicationInfo.SetIntAttr("VersionNumber", 1000);
	applicationInfo.InsertMultiAttr("KnownVersions", 1000);
	applicationInfo.InsertMultiAttr("KnownVersionNames", istd::CString("1.0.0"));
	applicationInfo.SetStringAttr("ApplicationName", "ImageView");
	applicationInfo.InitComponent();

	icomp::TSimComponentWrap<QtGuiPck::SplashScreen> splashScreenGui;
	splashScreenGui.SetStringAttr("ImagePath", "../../../Docs/Images/AcfSplashScreen.png");
	splashScreenGui.SetStringAttr("ProductType", "Tutorial");
	splashScreenGui.SetStringAttr("CopyrightText", "This is a part of ACF project.\nSee 'licence.txt' for copyright informations");
	splashScreenGui.SetRef("ApplicationInfo", &applicationInfo);
	splashScreenGui.InitComponent();

	icomp::TSimComponentsFactory<QtPck::Bitmap> modelFactoryComp;

	icomp::TSimComponentsFactory<QtViewPck::ImageView> viewFactoryComp;
	viewFactoryComp.SetBoolAttr("AllowWidgetResize", true);

	icomp::TSimComponentWrap<QtPck::BitmapLoader> bitmapLoaderComp;
	bitmapLoaderComp.InitComponent();

	icomp::TSimComponentWrap<QtPck::ExtendedDocumentTemplate> documentTemplateComp;
	documentTemplateComp.SetRef("AboutGui", &splashScreenGui);
	documentTemplateComp.SetFactory("DocumentFactory", &modelFactoryComp);
	documentTemplateComp.SetFactory("ViewFactory", &viewFactoryComp);
	documentTemplateComp.SetRef("DocumentLoader", &bitmapLoaderComp);
	documentTemplateComp.SetBoolAttr("IsEditSupported", false);
	documentTemplateComp.SetBoolAttr("IsNewSupported", false);
	documentTemplateComp.InitComponent();

	icomp::TSimComponentWrap<QtGuiPck::MultiDocWorkspaceGui> workspaceComp;
	workspaceComp.SetRef("DocumentTemplate", &documentTemplateComp);
	workspaceComp.InitComponent();

	icomp::TSimComponentWrap<QtGuiPck::MainWindowGui> mainWindowComp;
	mainWindowComp.SetRef("Workspace", &workspaceComp);
	mainWindowComp.SetRef("DocumentManager", &workspaceComp);
	mainWindowComp.InitComponent();

	application.SetRef("ApplicationInfo", &applicationInfo);
	application.SetRef("MainGui", &mainWindowComp);
	application.SetRef("SplashScreen", &splashScreenGui);
	application.InitComponent();

	return application.Execute(argc, argv);
}

