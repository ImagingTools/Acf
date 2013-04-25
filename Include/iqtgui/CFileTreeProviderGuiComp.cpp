#include "iqtgui/CFileTreeProviderGuiComp.h"


namespace iqtgui
{


// protected methods

// reimplemented (CGuiComponentBase)

void CFileTreeProviderGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	ibase::IQtItemModelProvider* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		FileList->setModel(const_cast<QAbstractItemModel*>(objectPtr->GetItemModel()));

		QItemSelectionModel* selectionModelPtr = FileList->selectionModel();
		if (selectionModelPtr != NULL){
			connect(selectionModelPtr,
				SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
				this,
				SLOT(OnSelectionChanged(const QItemSelection&, const QItemSelection&)));
		}
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CFileTreeProviderGuiComp::UpdateGui(int /*updateFlags*/)
{
	Q_ASSERT(IsGuiCreated());

	//ibase::IQtItemModelProvider* objectPtr = GetObjectPtr();
	//if (objectPtr != NULL){
	//	FileList->setModel(const_cast<QAbstractItemModel*>(objectPtr->GetItemModel()));
	//}
}


// private slots

void CFileTreeProviderGuiComp::OnSelectionChanged(const QItemSelection& selected, const QItemSelection&/* deselected*/)
{
	if (!selected.indexes().isEmpty() && m_currentFileCompPtr.IsValid()){
		m_currentFileCompPtr->SetPath(QString());

		ibase::IQtItemModelProvider* objectPtr = GetObjectPtr();
		if (objectPtr != NULL){
			const QAbstractItemModel* modelPtr = objectPtr->GetItemModel();
			if (modelPtr != NULL){
				QModelIndex selectedIndex = selected.indexes().at(0);

				QString currentFilePath = modelPtr->data(selectedIndex, Qt::UserRole+100).toString();
				QFileInfo fileInfo(currentFilePath);

				bool isFile = fileInfo.isFile();
				bool isDir = fileInfo.isDir();

				int selectedFileType = ifile::IFileNameParam::PT_UNKNOWN;

				if (isFile){
					selectedFileType = ifile::IFileNameParam::PT_FILE;
				}

				if (isDir){
					selectedFileType = ifile::IFileNameParam::PT_DIRECTORY;
				}

				int supportedPathType = m_currentFileCompPtr->GetPathType();
				if ((supportedPathType == ifile::IFileNameParam::PT_UNKNOWN) || (supportedPathType == selectedFileType)){
					m_currentFileCompPtr->SetPath(currentFilePath);
				}
			}
		}
	}
}


} // namespace iqtgui


