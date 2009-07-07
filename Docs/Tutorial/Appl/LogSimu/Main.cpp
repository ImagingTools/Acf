#include "icomp/TSimComponentWrap.h"
#include "icomp/TSimComponentsFactory.h"

#include "BasePck/BasePck.h"
#include "QtPck/QtPck.h"

#include "CLogClientGuiComp.h"


int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(iqtgui);

	QApplication::setStyle("plastique");

	icomp::TSimComponentWrap<QtPck::GuiApplication> application;
	application.InitializeApplication(argc, argv);

	icomp::TSimComponentWrap<QtPck::SplashScreen> splashScreenGui;
	splashScreenGui.SetStringAttr("ImagePath", "../../../Docs/Images/AcfSplashScreen.png");
	splashScreenGui.SetStringAttr("ProductName", "Log");
	splashScreenGui.SetStringAttr("ProductType", "Tutorial");
	splashScreenGui.SetStringAttr("CopyrightText", "This is a part of ACF project.\nSee 'licence.txt' for copyright informations");
	splashScreenGui.InitComponent();

	icomp::TSimComponentWrap<BasePck::XmlFileSerializer> logSerializerComp;
	logSerializerComp.InsertMultiAttr("FileExtensions", istd::CString("xml"));
	logSerializerComp.InsertMultiAttr("TypeDescriptions", istd::CString("Log File"));
	logSerializerComp.InitComponent();

	icomp::TSimComponentWrap<QtPck::FileDialogLoader> fileDialogLoaderComp;
	fileDialogLoaderComp.InsertMultiRef("Loaders", &logSerializerComp);
	fileDialogLoaderComp.InitComponent();

	icomp::TSimComponentWrap<QtPck::Log> log;
	log.SetIntAttr("MaxMessageCount", 10000);
	log.SetRef("Exporter", &fileDialogLoaderComp);
	log.InitComponent();

	icomp::TSimComponentWrap<CLogClientGuiComp> logClientGui;
	logClientGui.SetRef("Log", &log);
	logClientGui.SetDoubleAttr("AddIntervall", 0.01);
	logClientGui.InitComponent();

	application.SetRef("MainGui", &logClientGui);
	application.SetRef("SplashScreen", &splashScreenGui);
	application.InitComponent();

	return application.Execute(argc, argv);
}

