#ifndef iqtgui_CFileSystemExplorerGuiComp_included
#define iqtgui_CFileSystemExplorerGuiComp_included


// Qt includes
#include <QTreeWidget>
#include <QFileSystemModel>


// ACF includes
#include "iqtgui/TDesignerGuiCompBase.h"

#include "iqtgui/Generated/ui_CFileSystemExplorerGuiComp.h"


namespace iqtgui
{


/**
	Component for visualization of file system contents.
*/
class CFileSystemExplorerGuiComp: public iqtgui::TDesignerGuiCompBase<Ui::CFileSystemExplorerGuiComp>
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CFileSystemExplorerGuiComp> BaseClass; 
	
	I_BEGIN_COMPONENT(CFileSystemExplorerGuiComp);
		I_ASSIGN(m_fileFilterAttrPtr, "FileFilter", "File filter", false, "*.*");
		I_ASSIGN(m_showFileTypeAttrPtr, "ShowFileType", "Show file type description", false, false);
		I_ASSIGN(m_showFileModificationTimeAttrPtr, "ShowModificationTime", "Show last file modification time", false, false);
	I_END_COMPONENT;

protected:
	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

protected:
	I_ATTR(istd::CString, m_fileFilterAttrPtr);
	I_ATTR(bool, m_showFileTypeAttrPtr);
	I_ATTR(bool, m_showFileModificationTimeAttrPtr);

	QFileSystemModel m_fileSystemModel;
};


} // namespace iqtgui


#endif // !iqtgui_CFileSystemExplorerGuiComp_included

