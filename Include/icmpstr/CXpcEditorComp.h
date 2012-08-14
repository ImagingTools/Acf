#ifndef icmpstr_CXpcEditorComp_included
#define icmpstr_CXpcEditorComp_included


// Qt includes
#include <QtGui/QStringListModel>
#include <QtCore/QDir>


// ACF includes
#include "icomp/CXpcModel.h"

#include "idoc/IDocumentManager.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "Generated/ui_CXpcEditorComp.h"


namespace icmpstr
{


/**
	Editor for ACF package loader configuration.
*/
class CXpcEditorComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::XpcEditorComp, icomp::CXpcModel>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::XpcEditorComp, icomp::CXpcModel> BaseClass;

	I_BEGIN_COMPONENT(CXpcEditorComp);
		I_ASSIGN(m_docMgrPtr, "DocumentManager", "An object to provide current document file name, to handle relative paths", true, "DocumentManager");
		I_ASSIGN(m_confFileTypeInfo, "ConfigurationFileType", "File type description for filtering configuration files, ie. XPC", false, "ConfigurationFileType");
		I_ASSIGN(m_packageFileTypeInfo, "PackageFileType", "File type description for filtering package files, ie. ARP", false, "PackageFileType");
	I_END_COMPONENT;

	CXpcEditorComp();

protected:
	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();

private Q_SLOTS:
	void on_button_add_cf_clicked();
	void on_button_add_pd_clicked();
	void on_button_add_pk_clicked();
	void on_button_remove_cf_clicked();
	void on_button_remove_pd_clicked();
	void on_button_remove_pk_clicked();
	void on_buttonEdit_cf_clicked();
	void on_buttonEdit_pd_clicked();
	void on_buttonEdit_pk_clicked();
	void OnModelLayoutChanged();
	void OnModelDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight);
	void on_listView_cf_clicked(const QModelIndex & index);
	void on_listView_pd_clicked(const QModelIndex & index);
	void on_listView_pk_clicked(const QModelIndex & index);
	void on_listView_cf_doubleClicked(const QModelIndex & index);
	void on_lineEdit_cf_textEdited(const QString & text);
	void on_lineEdit_pd_textEdited(const QString & text);
	void on_lineEdit_pk_textEdited(const QString & text);

private:
	I_REF(idoc::IDocumentManager, m_docMgrPtr);
	I_REF(iser::IFileTypeInfo, m_confFileTypeInfo);
	I_REF(iser::IFileTypeInfo, m_packageFileTypeInfo);

	/**
		Retrieve strings from a table
	*/
	QStringList GetStringList(int tableIdx) const;

	/**
		Fill a table with strings (replaces existing content)
		\param table table index in m_tablePointer
	*/
	void SetStringList(int tableIdx, QStringList);

	QDir GetCurrentDocumentDir();

	/**
		Open a file dialog using a directory from currently selected file path
		If a user picks a file, its path replaces currently selected line.
		\param listView the MVC view to provide selection
		\param SelectDirectories whether to select directories instead of plain files
		\param typeInfo ACF file info structure to configure file filter
	*/
	void PickFileNameForSelection(QAbstractItemView* listView, bool SelectDirectories = false, iser::IFileTypeInfo* typeInfo = NULL);

	/**
		MVC models to handle data lists for tables
	*/
	QStringListModel *m_guiModel[3];

	enum Section
	{
		SectionConfFiles = 0, SectionPackageDirectories, SectionPackages
	};
};


} // namespace icmpstr


#endif // !icmpstr_CXpcEditorComp_included

