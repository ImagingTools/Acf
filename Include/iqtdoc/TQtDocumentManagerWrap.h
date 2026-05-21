// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QSettings>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QFileDialog>
#else
#include <QtGui/QFileDialog>
#endif

// ACF includes
#include <idoc/IDocumentManager.h>
#include <ifile/IFileNameParam.h>
#include <ifilegui/CFileDialogLoaderComp.h>


namespace iqtdoc
{


/**
	Wrapper for general functionality of a Qt based document workspace.
*/
template <class Base, class Gui>
class TQtDocumentManagerWrap: public Base, public Gui
{
public:
	typedef Base BaseClass;
	typedef typename BaseClass::Ids Ids;

	I_BEGIN_BASE_COMPONENT(TQtDocumentManagerWrap);
		I_ASSIGN(m_lastDirectoryParamCompPtr, "LastDirectoryParam", "Optional parameter for persistent storage of the last used directory", false, "LastDirectoryParam");
	I_END_COMPONENT;

	// pseudo-reimplemented (iqtgui::TRestorableGuiWrap)
	virtual void OnRestoreSettings(const QSettings& settings) override;
	virtual void OnSaveSettings(QSettings& settings) const override;

	// pseudo-reimplemented (idoc::CSingleDocumentManagerBase)
	virtual QString GetSaveFilePath(const QByteArray& documentTypeId, const istd::IChangeable* dataObjectPtr, const QString& currentFilePath) const override;

protected:
	/**
		Creates the filter for the file selection dialog.
		\param	documentTypeIdPtr	optional ID of document type if only filter for single document type should be created.
	*/
	virtual QStringList CreateFileDialogFilters(const QByteArray* documentTypeIdPtr, const istd::IChangeable* dataObjectPtr, int flags) const;

	void UpdateLastDirectory(const QString& filePath) const;
	virtual QStringList GetOpenFilePathesFromDialog(const QByteArray* documentTypeIdPtr) const;

	/**
		Get the last used directory path.
		If the IFileNameParam reference is valid, returns its path; otherwise returns the internally stored path.
	*/
	QString GetLastDirectory() const;

	I_REF(ifile::IFileNameParam, m_lastDirectoryParamCompPtr);

private:
	mutable QString m_lastDirectory;
};


// public methods

// pseudo-reimplemented (TRestorableGuiWrap)

template <class Base, class Gui>
void TQtDocumentManagerWrap<Base, Gui>::OnRestoreSettings(const QSettings& settings)
{
	if (m_lastDirectoryParamCompPtr.IsValid()){
		return;
	}

	QVariant valueNotSet = QVariant(-1);

	QVariant lastDirectory = settings.value("Workspace/LastOpenDocumentDirectory", valueNotSet);

	if (lastDirectory != valueNotSet){
		m_lastDirectory = lastDirectory.toString();
	}
}


template <class Base, class Gui>
void TQtDocumentManagerWrap<Base, Gui>::OnSaveSettings(QSettings& settings) const
{
	if (m_lastDirectoryParamCompPtr.IsValid()){
		return;
	}

	settings.setValue("Workspace/LastOpenDocumentDirectory", m_lastDirectory);
}


// pseudo-reimplemented (idoc::CSingleDocumentManagerBase)

template <class Base, class Gui>
QString TQtDocumentManagerWrap<Base, Gui>::GetSaveFilePath(const QByteArray& documentTypeId, const istd::IChangeable* dataObjectPtr, const QString& currentFilePath) const
{
	QStringList filters = CreateFileDialogFilters(&documentTypeId, dataObjectPtr, ifile::IFilePersistence::QF_FILE | ifile::IFilePersistence::QF_SAVE);

	QString defaultFilter = "*." + QFileInfo(currentFilePath).completeSuffix();
	for (QStringList::ConstIterator filterIter = filters.constBegin(); filterIter != filters.constEnd(); ++filterIter){
		if (filterIter->contains(defaultFilter)){
			defaultFilter = *filterIter;
			break;
		}
	}

	QString filePath = QFileDialog::getSaveFileName(
				Gui::GetWidget(),
				Gui::tr("Save..."),
				currentFilePath.isEmpty()? GetLastDirectory(): currentFilePath, filters.join(";;"),
				&defaultFilter);

	if (!filePath.isEmpty()){
		UpdateLastDirectory(filePath);
	}

	return filePath;
}


// protected methods

template <class Base, class Gui>
QStringList TQtDocumentManagerWrap<Base, Gui>::CreateFileDialogFilters(const QByteArray* documentTypeIdPtr, const istd::IChangeable* dataObjectPtr, int flags) const
{
	QStringList filters;

	Ids docTypeIds = BaseClass::GetDocumentTypeIds();

	QStringList allExt;

	if (documentTypeIdPtr != NULL){
		ifile::IFileTypeInfo* typeInfoPtr = BaseClass::GetDocumentFileTypeInfo(*documentTypeIdPtr);
		if (typeInfoPtr != NULL){
			ifilegui::CFileDialogLoaderComp::AppendLoaderFilterList(*typeInfoPtr, dataObjectPtr, flags, allExt, filters, false);
		}
	}
	else{
		for (		typename Ids::const_iterator docTypeIter = docTypeIds.begin();
					docTypeIter != docTypeIds.end();
					++docTypeIter){
			ifile::IFileTypeInfo* typeInfoPtr = BaseClass::GetDocumentFileTypeInfo(*docTypeIter);
			if (typeInfoPtr != NULL){
				ifilegui::CFileDialogLoaderComp::AppendLoaderFilterList(*typeInfoPtr, dataObjectPtr, flags, allExt, filters, docTypeIds.count() > 1);
			}
		}
	}

	if ((allExt.size() > 1) && ((flags & ifile::IFilePersistence::QF_SAVE) == 0)){
		filters.prepend(Gui::tr("All known documents (%1)").arg("*." + allExt.join(" *.")));
	}

	return filters;
}


template <class Base, class Gui>
void TQtDocumentManagerWrap<Base, Gui>::UpdateLastDirectory(const QString& filePath) const
{
	QFileInfo fileInfo(filePath);

	QString dirPath = fileInfo.dir().absolutePath();

	if (m_lastDirectoryParamCompPtr.IsValid()){
		m_lastDirectoryParamCompPtr->SetPath(dirPath);
	}
	else{
		m_lastDirectory = dirPath;
	}
}


template <class Base, class Gui>
QStringList TQtDocumentManagerWrap<Base, Gui>::GetOpenFilePathesFromDialog(const QByteArray* documentTypeIdPtr) const
{
	QStringList filters = CreateFileDialogFilters(documentTypeIdPtr, NULL, ifile::IFilePersistence::QF_FILE | ifile::IFilePersistence::QF_LOAD);

	QString lastUsedDirectory = GetLastDirectory();

	QFileInfo fileInfo(lastUsedDirectory);
	if (!fileInfo.exists()){
		lastUsedDirectory.clear();
	}

	return QFileDialog::getOpenFileNames(Gui::GetWidget(), Gui::tr("Open Files..."), lastUsedDirectory, filters.join(";;"));
}


template <class Base, class Gui>
QString TQtDocumentManagerWrap<Base, Gui>::GetLastDirectory() const
{
	if (m_lastDirectoryParamCompPtr.IsValid()){
		return m_lastDirectoryParamCompPtr->GetPath();
	}

	return m_lastDirectory;
}


} // namespace iqtdoc


