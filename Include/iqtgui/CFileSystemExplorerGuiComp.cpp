// Qt includes
#include <QHeaderView>


// ACF includes
#include "iqtgui/CFileSystemExplorerGuiComp.h"


namespace iqtgui
{


// public methods

// protected methods

// reimplemented (CGuiComponentBase)

void CFileSystemExplorerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	m_fileSystemModel.setRootPath(QDir::currentPath());

	FileTree->setModel(&m_fileSystemModel);

	FileTree->setDragDropMode(QTreeView::DragDrop);
	FileTree->setDragEnabled(true);

	QHeaderView* headerPtr = FileTree->header();
	if (headerPtr != NULL){
		if (m_showFileTypeAttrPtr.IsValid() && *m_showFileTypeAttrPtr){
			headerPtr->setSectionHidden(1, false);
		}
		else{
			headerPtr->setSectionHidden(1, true);
		}

		headerPtr->setSectionHidden(2, true);

		if (m_showFileModificationTimeAttrPtr.IsValid() && *m_showFileModificationTimeAttrPtr){
			headerPtr->setSectionHidden(3, false);
		}
		else{
			headerPtr->setSectionHidden(3, true);
		}
	}
}


void CFileSystemExplorerGuiComp::OnGuiDestroyed()
{
	BaseClass::OnGuiDestroyed();
}


} // namespace iqtgui


