#ifndef iqtdoc_CMultiDocumentWorkspaceGuiComp_included
#define iqtdoc_CMultiDocumentWorkspaceGuiComp_included


#include <QWorkspace>


#include "idoc/CDocumentManagerBase.h"

#include "iqt/TGuiComponentBase.h"


namespace iqmain
{


class IObserver;


/**	
	This class is a Qt-based workspace implementation of a document manager.
*/
class CMultiDocumentWorkspaceGuiComp:	public iqt::TGuiComponentBase<QWorkspace>, 
										public idoc::CDocumentManagerBase
{
	Q_OBJECT

public:
	typedef iqt::TGuiComponentBase<QWorkspace> BaseClass;
	typedef idoc::CDocumentManagerBase BaseClass2;

	I_BEGIN_COMPONENT(CMultiDocumentWorkspaceGuiComp)
		I_REGISTER_INTERFACE(idoc::IDocumentManager);
		I_ASSIGN_MULTI_0(m_documentTemplatesCompPtr, "DocumentTemplates", "Document templates", true)
		I_ASSIGN_MULTI_0(m_documentIdAttrPtr, "DocumentIds", "Document Ids", true)
		I_ASSIGN(m_scrollingEnabledAttrPtr, "ScrollingWorkspace", "Enable scorilling of workspace area", true, false)
	I_END_COMPONENT

	// reimplemented (iqt::IGuiObject)
	virtual void OnTryClose(bool* ignoredPtr = NULL);

	// reimplemented (idoc::IDocumentManager)
	virtual idoc::IDocument* OnFileNew(const std::string& documentId);
	virtual istd::CStringList GetDocumentIds() const;

protected:
	virtual bool eventFilter(QObject* obj, QEvent* event);

	// reimplemented (idoc::CDocumentManagerBase)
	virtual istd::CString GetSaveFileName(const idoc::IDocumentTemplate& documentTemplate) const;
	virtual istd::CStringList GetOpenFileNames(const idoc::IDocumentTemplate& documentTemplate) const;

	// reimplemented (imod::CMultiModelObserverBase)
	void OnUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (iqt:CGuiComponentBase)
	virtual void OnGuiCreated();	
	virtual void OnGuiDestroyed();

private:
	/**
		Creates signal/slot connnections for the implementation.
	*/
	virtual void CreateConnections();

	/**
		Returns the document index for the view \c view.
	*/
	virtual int GetDocumentIndex(const imod::IObserver* view) const;

	/**
		Creates the filter for the file selection dialog.
		The filter corresponds with the file types for \c documentID
	*/
	QString CreateFileDialogFilter(const idoc::IDocumentTemplate& documentTemplate) const;

protected slots:
	void OnWindowActivated(QWidget* window);

protected:
	I_MULTIREF(idoc::IDocumentTemplate, m_documentTemplatesCompPtr);
	I_MULTI_ATTR(istd::CString, m_documentIdAttrPtr);
	I_ATTR(bool, m_scrollingEnabledAttrPtr);
};


} // namespace iqmain


#endif // iqtdoc_CMultiDocumentWorkspaceGuiComp_included

