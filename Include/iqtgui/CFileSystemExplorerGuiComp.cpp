// Qt includes
#include <QHeaderView>
#include <QToolButton>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt/CFileSystem.h"

#include "iqtgui/CFileSystemExplorerGuiComp.h"


namespace iqtgui
{


// public methods

// reimplemented (imod::IModelEditor)

void CFileSystemExplorerGuiComp::UpdateModel() const
{
}


void CFileSystemExplorerGuiComp::UpdateEditor(int /*updateFlags*/)
{
	iprm::IFileNameParam* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		QModelIndex index = m_fileSystemModel.index(iqt::CFileSystem::GetEnrolledPath(iqt::GetQString(objectPtr->GetPath())));
		if (index.isValid()){
			QItemSelectionModel* selectionModelPtr = FileTree->selectionModel();
			if (selectionModelPtr != NULL){
				selectionModelPtr->select(index, QItemSelectionModel::Select);
			}
		}
	}
}


// protected methods

QStringList CFileSystemExplorerGuiComp::GetDefaultFilters() const
{
	QStringList retVal;

	if (m_filterInfoCompPtr.IsValid()){
		istd::CStringList extensions;
		if (m_filterInfoCompPtr->GetFileExtensions(extensions, iser::IFileTypeInfo::QF_NO_SAVING)){
			for (		istd::CStringList::const_iterator iter = extensions.begin();
						iter != extensions.end();
						++iter){
				retVal += "*." + iqt::GetQString(*iter);
			}
		}
	}

	return retVal;
}


// reimplemented (CGuiComponentBase)

void CFileSystemExplorerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_previewGuiCompPtr.IsValid()){
		m_previewGuiCompPtr->CreateGui(PreviewFrame);
	}

	FileTree->setModel(&m_fileSystemModel);
	m_fileSystemModel.setRootPath(QDir::currentPath());
	FileTree->setRootIndex(m_fileSystemModel.setRootPath(m_fileSystemModel.myComputer().toString()));

	QItemSelectionModel* selectionModelPtr = FileTree->selectionModel();
	if (selectionModelPtr != NULL){
		connect(selectionModelPtr, SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(OnSelectionChanged(const QItemSelection&, const QItemSelection&)));
	}

	QStringList fileFilters = GetDefaultFilters();

	m_fileSystemModel.setNameFilters(fileFilters);

	FilterFrame->setVisible(*m_showUserFilterAttrPtr);
	QLayout* filterLayoutPtr = FilterFrame->layout();
	if (filterLayoutPtr != NULL){
		m_filterEdit = new iqtgui::CExtLineEdit(tr("Enter text to filter file items"), 2, FilterFrame);

		// add "clear" button:
		QToolButton* clearButton = new QToolButton(m_filterEdit);
		clearButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
		clearButton->setIcon(QIcon(":/Icons/Clear"));
		connect(clearButton, SIGNAL(clicked()), m_filterEdit, SLOT(clear()));

		m_filterEdit->AddWidget(clearButton, Qt::AlignRight);

		filterLayoutPtr->addWidget(m_filterEdit);

		connect(m_filterEdit, SIGNAL(editingFinished()), this, SLOT(OnFilterChanged()));
	}

	FileTree->setDragDropMode(QTreeView::DragDrop);
	FileTree->setDragEnabled(true);

	QHeaderView* headerPtr = FileTree->header();
	if (headerPtr != NULL){
		if (*m_showFileTypeAttrPtr){
			headerPtr->setSectionHidden(2, false);
		}
		else{
			headerPtr->setSectionHidden(2, true);
		}

		headerPtr->setSectionHidden(1, true);

		if (*m_showFileModificationTimeAttrPtr){
			headerPtr->setSectionHidden(3, false);
		}
		else{
			headerPtr->setSectionHidden(3, true);
		}
	}
}


void CFileSystemExplorerGuiComp::OnGuiDestroyed()
{
	if (m_previewGuiCompPtr.IsValid()){
		m_previewGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


// private slots:

void CFileSystemExplorerGuiComp::OnFilterChanged()
{
	QString filterText = m_filterEdit->GetText();

	QStringList fileFilters = GetDefaultFilters();

	if (!filterText.isEmpty()){
		fileFilters << filterText;
	}

	m_fileSystemModel.setNameFilters(fileFilters);
}


void CFileSystemExplorerGuiComp::OnSelectionChanged(const QItemSelection& selected, const QItemSelection&/* deselected*/)
{
	if (!selected.indexes().isEmpty()){
		QModelIndex selectedIndex = selected.indexes().at(0);

		QString currentFilePath = m_fileSystemModel.filePath(selectedIndex);

		iprm::IFileNameParam* objectPtr = GetObjectPtr();
		if (objectPtr != NULL){
			objectPtr->SetPath(iqt::GetCString(currentFilePath));
		}
	}
}


} // namespace iqtgui


