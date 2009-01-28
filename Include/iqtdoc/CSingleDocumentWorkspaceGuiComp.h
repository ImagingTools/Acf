#ifndef iqtdoc_CSingleDocumentWorkspaceGuiComp_included
#define iqtdoc_CSingleDocumentWorkspaceGuiComp_included


// Qt includes
#include <QWidget>

// ACF includes
#include "idoc/ICommandsProvider.h"
#include "idoc/CSingleDocumentManagerBase.h"

#include "iqtgui/TGuiComponentBase.h"
#include "iqtgui/CHierarchicalCommand.h"


namespace iqtdoc
{


/**	
	This class is a Qt-based workspace implementation of a document manager.
*/
class CSingleDocumentWorkspaceGuiComp:
			public iqtgui::TGuiComponentBase<QWidget>, 
			public idoc::CSingleDocumentManagerBase
{
public:
	typedef iqtgui::TGuiComponentBase<QWidget> BaseClass;
	typedef idoc::CSingleDocumentManagerBase BaseClass2;

	I_BEGIN_COMPONENT(CSingleDocumentWorkspaceGuiComp)
		I_REGISTER_INTERFACE(idoc::IDocumentManager)
		I_ASSIGN(m_documentTemplateCompPtr, "DocumentTemplate", "Document template", true, "DocumentTemplate");
	I_END_COMPONENT;

	enum GroupId
	{
		GI_WINDOW = 0x300,
		GI_DOCUMENT,
		GI_VIEW
	};

	CSingleDocumentWorkspaceGuiComp();

	// reimplemented (iqtgui::IGuiObject)
	virtual void OnTryClose(bool* ignoredPtr = NULL);

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

protected:
	/**
		Update titles of views or all views of specified document.
		\param	optional document object, if you want to update only views of single document.
	*/
	void UpdateTitle();

	/**
		Creates the filter for the file selection dialog.
		\param	documentTypeIdPtr	optional ID of document type if only filter for single document type should be created.
	*/
	QString CreateFileDialogFilter(const std::string* documentTypeIdPtr = NULL) const;

	// reimplemented (idoc::CSingleDocumentManagerBase)
	virtual istd::CString GetOpenFileName(const std::string* documentTypeIdPtr = NULL) const;
	virtual istd::CString GetSaveFileName(const std::string& documentTypeId) const;
	virtual void OnViewRegistered(istd::IPolymorphic* viewPtr);
	virtual void OnViewRemoved(istd::IPolymorphic* viewPtr);
	virtual void QueryDocumentClose(bool* ignoredPtr);

	// reimplemented (iqt:CGuiComponentBase)
	virtual void OnGuiCreated();	
	virtual void OnGuiDestroyed();

	// reimplemented (istd:IChangeable)
	virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

private:
	void UpdateLastDirectory(const QString& filePath) const;

	I_REF(idoc::IDocumentTemplate, m_documentTemplateCompPtr);

	iqtgui::IGuiObject* m_lastViewPtr;

	mutable QString m_lastDirectory;
};


} // namespace iqtdoc


#endif // !iqtdoc_CSingleDocumentWorkspaceGuiComp_included

