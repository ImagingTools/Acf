#ifndef icmpstr_CQuickHelpViewerComp_included
#define icmpstr_CQuickHelpViewerComp_included


// Qt includes
#include <QTextBrowser>


// ACF includes
#include "iprm/IFileNameParam.h"
#include "idoc/IHelpViewer.h"
#include "idoc/IHelpFileProvider.h"
#include "iqtgui/TDesignerGuiCompBase.h"

#include "Generated/ui_CQuickHelpViewerComp.h"


namespace icmpstr
{


class CQuickHelpViewerComp:
			public iqtgui::TDesignerGuiCompBase<Ui::CQuickHelpViewerComp>,
			virtual public idoc::IHelpViewer
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CQuickHelpViewerComp> BaseClass;

	I_BEGIN_COMPONENT(CQuickHelpViewerComp);
		I_REGISTER_INTERFACE(idoc::IHelpViewer);
		I_ASSIGN(m_descriptionFileProviderCompPtr, "HelpFileProvider", "Calculate path of html document for short description", true, "HelpFileProvider");
		I_ASSIGN(m_technicalFileProviderCompPtr, "HelpFileProvider", "Calculate path of html document for technical description", true, "HelpFileProvider");
		I_ASSIGN(m_docuEditorFileParamsCompPtr, "DocuEditorFileParam", "Object storing path to html editor used to edit the description", true, "DocuEditorFileParam");
		I_ASSIGN(m_techDocuViewerFileParamsCompPtr, "TechDocuViewerParam", "Object storing path to html viewer used to show technical documentation", true, "TechDocuViewerParam");
	I_END_COMPONENT;

	// reimplemented (idoc::IHelpViewer)
	virtual void ShowHelp(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr);

protected slots:
	void on_EditButton_clicked();
	void on_NewButton_clicked();
	void on_ShowTechButton_clicked();

private:
	I_REF(idoc::IHelpFileProvider, m_descriptionFileProviderCompPtr);
	I_REF(idoc::IHelpFileProvider, m_technicalFileProviderCompPtr);
	I_REF(iprm::IFileNameParam, m_docuEditorFileParamsCompPtr);
	I_REF(iprm::IFileNameParam, m_techDocuViewerFileParamsCompPtr);

	istd::CString m_descrFilePath;
	istd::CString m_techFilePath;
};


} // namespace icmpstr


#endif // !icmpstr_CQuickHelpViewerComp_included


