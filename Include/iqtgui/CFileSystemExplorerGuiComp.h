#ifndef iqtgui_CFileSystemExplorerGuiComp_included
#define iqtgui_CFileSystemExplorerGuiComp_included


// Qt includes
#include <QTreeWidget>
#include <QFileSystemModel>
#include <QItemSelectionModel>


// ACF includes
#include "iprm/IFileNameParam.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtgui/CExtLineEdit.h"

#include "iqtgui/Generated/ui_CFileSystemExplorerGuiComp.h"


namespace iqtgui
{


/**
	Component for visualization of file system contents.
*/
class CFileSystemExplorerGuiComp:
		public iqtgui::TDesignerGuiObserverCompBase<
					Ui::CFileSystemExplorerGuiComp, iprm::IFileNameParam>
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
					Ui::CFileSystemExplorerGuiComp, iprm::IFileNameParam> BaseClass;
	
	I_BEGIN_COMPONENT(CFileSystemExplorerGuiComp);
		I_ASSIGN(m_fileFilterAttrPtr, "FileFilter", "File filter", false, "*.*");
		I_ASSIGN(m_showFileTypeAttrPtr, "ShowFileType", "Show file type description", false, false);
	I_END_COMPONENT;

public:
	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

protected:
	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private Q_SLOTS:
	void OnFilterChanged();
	void OnSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

private:
	I_ATTR(istd::CString, m_fileFilterAttrPtr);
	I_ATTR(bool, m_showFileTypeAttrPtr);
	I_ATTR(bool, m_showFileModificationTimeAttrPtr);

	QFileSystemModel m_fileSystemModel;

	iqtgui::CExtLineEdit* m_filterEdit;
};


} // namespace iqtgui


#endif // !iqtgui_CFileSystemExplorerGuiComp_included

