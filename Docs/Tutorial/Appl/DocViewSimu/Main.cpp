#include "icomp/TSimComponentWrap.h"
#include "icomp/TSimComponentsFactory.h"


#include "BasePck/BasePck.h"
#include "QtPck/QtPck.h"

#include "TutorialPck/TutorialPck.h"


int main(int argc, char *argv[])
{
	icomp::TSimComponentWrap<QtPck::GuiApplication> application;
	application.SetDoubleAttr("SplashTime", 1.5);
	application.EnsureInitialized(argc, argv);

	Q_INIT_RESOURCE(iqtgui);

	icomp::TSimComponentWrap<BasePck::ApplicationInfo> applicationInfo;
	applicationInfo.SetIntAttr("VersionNumber", 1000);
	applicationInfo.InsertMultiAttr("KnownVersions", 1000);
	applicationInfo.InsertMultiAttr("KnownVersionNames", istd::CString("1.0.0"));
	applicationInfo.SetStringAttr("ApplicationName", "DocView");
	applicationInfo.InitComponent();

	icomp::TSimComponentWrap<QtPck::SplashScreen> splashScreenGui;
	splashScreenGui.SetStringAttr("ImagePath", "../../../Docs/Images/AcfSplashScreen.png");
	splashScreenGui.SetStringAttr("ProductType", "Tutorial");
	splashScreenGui.SetStringAttr("CopyrightText", "This is a part of ACF project.\nSee 'licence.txt' for copyright informations");
	splashScreenGui.SetRef("ApplicationInfo", &applicationInfo);
	splashScreenGui.InitComponent();

	icomp::TSimComponentsFactory<BasePck::TextDocument> modelFactoryComp;
	modelFactoryComp.SetStringAttr("DefaultText", "Ahoj przygodo!");

	icomp::TSimComponentsFactory<TutorialPck::TextEditor> viewFactoryComp;

	icomp::TSimComponentWrap<BasePck::BinaryFileSerializer> serializerComp;
	serializerComp.InsertMultiAttr("FileExtensions", istd::CString("bin"));
	serializerComp.InsertMultiAttr("TypeDescriptions", istd::CString("Binary text files"));
	serializerComp.InitComponent();

	icomp::TSimComponentWrap<QtPck::ExtendedDocumentTemplate> documentTemplateComp;
	documentTemplateComp.SetFactory("DocumentFactory", &modelFactoryComp);
	documentTemplateComp.SetFactory("ViewFactory", &viewFactoryComp);
	documentTemplateComp.SetRef("DocumentLoader", &serializerComp);
	documentTemplateComp.SetBoolAttr("IsEditSupported", true);
	documentTemplateComp.SetBoolAttr("IsNewSupported", true);
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

