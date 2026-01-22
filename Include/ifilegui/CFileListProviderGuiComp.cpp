// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifilegui/CFileListProviderGuiComp.h>


namespace ifilegui
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CFileListProviderGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	ifile::IFileListProvider* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		FileList->clear();

		const QFileInfoList& fileList = objectPtr->GetFileList();

		for (int fileIndex = 0; fileIndex < fileList.count(); fileIndex++){
			QTreeWidgetItem* fileItemPtr = new QTreeWidgetItem();

			fileItemPtr->setText(0, fileList[fileIndex].absoluteFilePath());

			FileList->addTopLevelItem(fileItemPtr);
		}
	}
}


} // namespace ifilegui


