#ifndef iqtgui_CFileTreeViewGuiComp_included
#define iqtgui_CFileTreeViewGuiComp_included


// Qt includes
#include <QtCore/QDir>
#include <QtGui/QStandardItemModel>
#include <QtGui/QFileIconProvider>


// ACF includes
#include "ifile/IFileNameParam.h"
#include "ifile/IFileTypeInfo.h"

#include "ibase/IQtItemModelProvider.h"
#include "imod/CMultiModelDispatcherBase.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtgui/Generated/ui_CFileTreeViewGuiComp.h"


namespace iqtgui
{


class CFileTreeViewGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CFileTreeViewGuiComp, ifile::IFileNameParam>,
			public imod::CMultiModelDispatcherBase
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CFileTreeViewGuiComp, ifile::IFileNameParam> BaseClass;

	I_BEGIN_COMPONENT(CFileTreeViewGuiComp);
		I_ASSIGN(m_currentFileCompPtr, "CurrentFile", "Write name of the currently selected file to", false, "CurrentFile");
		I_ASSIGN_TO(m_currentFileModelCompPtr, m_currentFileCompPtr, false);
		I_ASSIGN(m_fileTypeInfoCompPtr, "FileTypeInfo", "File type info used to create file filters", false, "FileTypeInfo");
		I_ASSIGN_MULTI_0(m_filtersAttrPtr, "Filters", "List of filters if no loader is specified", false);
	I_END_COMPONENT;

	enum DataRoles{
		DR_PATH = Qt::UserRole + 1
	};

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr);

private Q_SLOTS:
	void OnSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
	void on_Refresh_clicked();

private:
	void RebuildTreeModel();
	void UpdateCurrentSelection();

	/**
		Creates the list of files in a root directory \c root.
		Several filters can be applied to the \c root before call of this function.
		So you can do a separate filtering of files and directories.
	*/
	bool CreateFileList(const QDir& root,
		const QStringList& nameFilters,
		QDir::SortFlags sortSpec,
		QStandardItem* parentItemPtr);

	/**
		Several filters can be applied to the QDir object before call of this function.
	*/
	bool CreateDirectoryList(
		const QDir& root,
		const QStringList& nameFilters,
		QDir::SortFlags sortSpec,
		QStandardItem* parentItemPtr);

	void EnumerateDirectory(
		const QDir& root,
		const QStringList& nameFilters,
		QDir::SortFlags sortSpec,
		QStandardItem* parentItemPtr);

private:
	bool m_fileModelUpdateAllowed;

	mutable QStandardItemModel m_itemModel;
	QFileIconProvider m_iconProvider;

	I_REF(ifile::IFileNameParam, m_currentFileCompPtr);
	I_REF(imod::IModel, m_currentFileModelCompPtr);
	I_REF(ifile::IFileTypeInfo, m_fileTypeInfoCompPtr);
	I_MULTIATTR(QString, m_filtersAttrPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CFileTreeProviderGuiComp_included


