
#include "imod/TModelWrap.h"

#include "icomp/TSimComponentWrap.h"

#include "iqt/CApplicationComp.h"

#include "iqmain/CMainWindowGuiComp.h"
#include "iqmain/CMultiDocumentWorkspaceGuiComp.h"

#include "istdc/TSingleFactoryComp.h"

#include "idoc/TDocumentTemplateCompWrap.h"
#include "idoc/CSerializableDocument.h"
#include "idoc/CMultiDocumentTemplate.h"


int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(iqmain);

	// document factory:
//	typedef istdc::TSingleFactoryComp<imod::IModel, imod::TModelWrap<> > DocumentFactoryComp;
//	icomp::TSimComponentWrap<DocumentFactoryComp> documentFactoryComp;
//	documentFactoryComp.InitComponent();

	typedef idoc::TDocumentTemplateCompWrap<idoc::CMultiDocumentTemplate> DocumentTemplateComp;
	icomp::TSimComponentWrap<DocumentTemplateComp> documentTemplateComp;
	documentTemplateComp.InitComponent();

	icomp::TSimComponentWrap<imod::TModelWrap<iqmain::CMultiDocumentWorkspaceGuiComp> > workspaceComp;
	workspaceComp.SetRef("DocumentTemplates", &documentTemplateComp);
	workspaceComp.InitComponent();

	icomp::TSimComponentWrap<iqmain::CMainWindowGuiComp> mainWindowComp;
	mainWindowComp.SetRef("Workspace", &workspaceComp);
	mainWindowComp.SetRef("DocumentManager", &workspaceComp);
	mainWindowComp.InitComponent();

	icomp::TSimComponentWrap<iqt::CApplicationComp> application;
	application.SetRef("MainGui", &mainWindowComp);
	application.InitComponent();

	return application.Execute(argc, argv);
}

