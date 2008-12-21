#include "icomp/TSimComponentWrap.h"
#include "icomp/TSimComponentsFactory.h"


#include "QtPck/QtPck.h"
#include "BasePck/BasePck.h"


int main(int argc, char *argv[])
{
	icomp::TSimComponentWrap<QtPck::GuiApplication> application;
	application.EnsureInitialized(argc, argv);

	Q_INIT_RESOURCE(iqtgui);

	icomp::TSimComponentWrap<BasePck::ApplicationInfoExt> applicationInfo;
	applicationInfo.SetIntAttr("VersionNumber", 1000);
	applicationInfo.InsertMultiAttr("KnownVersions", 1000);
	applicationInfo.InsertMultiAttr("KnownVersionNames", istd::CString("1.0.0"));
	applicationInfo.SetStringAttr("ApplicationName", "ImageView");
	applicationInfo.InitComponent();

	icomp::TSimComponentWrap<QtPck::SplashScreen> splashScreenGui;
	splashScreenGui.SetStringAttr("ImagePath", "../../../Docs/Images/AcfSplashScreen.png");
	splashScreenGui.SetStringAttr("ProductType", "Tutorial");
	splashScreenGui.SetStringAttr("CopyrightText", "This is a part of ACF project.\nSee 'licence.txt' for copyright informations");
	splashScreenGui.SetRef("ApplicationInfo", &applicationInfo);
	splashScreenGui.InitComponent();

	icomp::TSimComponentsFactory<QtPck::Bitmap> modelFactoryComp;
	icomp::TSimComponentsFactory<QtPck::ImageView> viewFactoryComp;
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

	icomp::TSimComponentWrap<QtPck::MultiDocWorkspaceGui> workspaceComp;
	workspaceComp.SetRef("DocumentTemplate", &documentTemplateComp);
	workspaceComp.InitComponent();

	icomp::TSimComponentWrap<QtPck::MainWindowGui> mainWindowComp;
	mainWindowComp.SetRef("Workspace", &workspaceComp);
	mainWindowComp.SetRef("DocumentManager", &workspaceComp);
	mainWindowComp.InitComponent();

	application.SetRef("ApplicationInfo", &applicationInfo);
	application.SetRef("MainGui", &mainWindowComp);
	application.SetRef("SplashScreen", &splashScreenGui);
	application.InitComponent();

	return application.Execute(argc, argv);
}

