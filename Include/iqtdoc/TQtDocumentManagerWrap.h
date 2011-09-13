#ifndef iqtdoc_TQtDocumentManagerWrap_included
#define iqtdoc_TQtDocumentManagerWrap_included


// Qt includes
#include <QPrintDialog>
#include <QFileDialog>
#include <QSettings>


// ACF includes
#include "idoc/IDocumentManager.h"

#include "iqtgui/CFileDialogLoaderComp.h"

#include "iqtdoc/IPrintable.h"


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

	// pseudo-reimplemented (iqtgui::TRestorableGuiWrap)
	virtual void OnRestoreSettings(const QSettings& settings);
	virtual void OnSaveSettings(QSettings& settings) const;

	// pseudo-reimplemented (idoc::IDocumentManager)
	virtual int GetAllowedOperationFlags(const istd::IPolymorphic* viewPtr = NULL) const;
	virtual void FilePrint(int documentIndex = -1) const;

	// pseudo-reimplemented (idoc::CSingleDocumentManagerBase)
	virtual istd::CString GetSaveFilePath(const std::string& documentTypeId) const;

protected:
	/**
		Creates the filter for the file selection dialog.
		\param	documentTypeIdPtr	optional ID of document type if only filter for single document type should be created.
	*/
	QString CreateFileDialogFilter(const std::string* documentTypeIdPtr = NULL, int flags = 0) const;

	void UpdateLastDirectory(const QString& filePath) const;
	virtual QStringList GetOpenFilePathesFromDialog(const std::string* documentTypeIdPtr) const;

private:
	mutable QString m_lastDirectory;
};


// public methods

// pseudo-reimplemented (TRestorableGuiWrap)

template <class Base, class Gui>
void TQtDocumentManagerWrap<Base, Gui>::OnRestoreSettings(const QSettings& settings)
{
	QVariant valueNotSet = QVariant(-1);

	QVariant lastDirectory = settings.value("Workspace/LastOpenDocumentDirectory", valueNotSet);

	if (lastDirectory != valueNotSet){
		m_lastDirectory = lastDirectory.toString();
	}
}


template <class Base, class Gui>
void TQtDocumentManagerWrap<Base, Gui>::OnSaveSettings(QSettings& settings) const
{
	settings.setValue("Workspace/LastOpenDocumentDirectory", m_lastDirectory);
}


// reimplemented (idoc::IDocumentManager)

template <class Base, class Gui>
int TQtDocumentManagerWrap<Base, Gui>::GetAllowedOperationFlags(const istd::IPolymorphic* viewPtr) const
{
	int retVal = BaseClass::GetAllowedOperationFlags(viewPtr);

	if (viewPtr == NULL){
		viewPtr = BaseClass::GetActiveView();
	}

	if (dynamic_cast<const iqtdoc::IPrintable*>(viewPtr) != NULL){
		retVal |= idoc::IDocumentManager::OF_FILE_PRINT;
	}

	return retVal;
}


template <class Base, class Gui>
void TQtDocumentManagerWrap<Base, Gui>::FilePrint(int documentIndex) const
{
	istd::IPolymorphic* activeViewPtr = NULL;
	if (documentIndex >= 0){
		I_ASSERT(documentIndex < GetDocumentsCount());

		if (BaseClass::GetViewsCount(documentIndex) > 0){
			activeViewPtr = BaseClass::GetViewFromIndex(documentIndex, 0);
		}
	}
	else{
		activeViewPtr = BaseClass::GetActiveView();
	}

	if (activeViewPtr != NULL){
		iqtdoc::IPrintable* printablePtr = dynamic_cast<iqtdoc::IPrintable*>(activeViewPtr);
		if (printablePtr != NULL){
			QPrinter printer(QPrinter::HighResolution);
			QPrintDialog printerDialog(&printer);

			if (printerDialog.exec() == QDialog::Accepted){
				printablePtr->Print(printerDialog.printer());
			}
		}
	}
}


// pseudo-reimplemented (idoc::CSingleDocumentManagerBase)

template <class Base, class Gui>
istd::CString TQtDocumentManagerWrap<Base, Gui>::GetSaveFilePath(const std::string& documentTypeId) const
{
	QString filter = CreateFileDialogFilter(&documentTypeId, iser::IFileLoader::QF_FILE | iser::IFileLoader::QF_SAVE);

	QString filePath = QFileDialog::getSaveFileName(NULL, Gui::tr("Save..."), m_lastDirectory, filter);

	UpdateLastDirectory(filePath);

	return iqt::GetCString(filePath);
}


// protected methods

template <class Base, class Gui>
QString TQtDocumentManagerWrap<Base, Gui>::CreateFileDialogFilter(const std::string* documentTypeIdPtr, int flags) const
{
	QString retVal;

	const idoc::IDocumentTemplate* templatePtr = BaseClass::GetDocumentTemplate();
	if (templatePtr != NULL){
		idoc::IDocumentTemplate::Ids docTypeIds = templatePtr->GetDocumentTypeIds();

		QString allExt;
		int filtersCount = 0;

		if (documentTypeIdPtr != NULL){
			iser::IFileLoader* loaderPtr = templatePtr->GetFileLoader(*documentTypeIdPtr);
			if (loaderPtr != NULL){
				filtersCount += iqtgui::CFileDialogLoaderComp::AppendLoaderFilterList(*loaderPtr, flags, allExt, retVal);
			}
		}
		else{
			for (		idoc::IDocumentTemplate::Ids::const_iterator docTypeIter = docTypeIds.begin();
						docTypeIter != docTypeIds.end();
						++docTypeIter){
				iser::IFileLoader* loaderPtr = templatePtr->GetFileLoader(*docTypeIter);
				if (loaderPtr != NULL){
					filtersCount += iqtgui::CFileDialogLoaderComp::AppendLoaderFilterList(*loaderPtr, flags, allExt, retVal);
				}
			}
		}

		if ((filtersCount > 1) && ((flags & iser::IFileLoader::QF_SAVE) == 0)){
			retVal = Gui::tr("All known documents (%1)\n").arg(allExt) + retVal;
		}
	}

	return retVal;
}

template <class Base, class Gui>
void TQtDocumentManagerWrap<Base, Gui>::UpdateLastDirectory(const QString& filePath) const
{
	QFileInfo fileInfo(filePath);

	m_lastDirectory = fileInfo.dir().absolutePath();
}


template <class Base, class Gui>
QStringList TQtDocumentManagerWrap<Base, Gui>::GetOpenFilePathesFromDialog(const std::string* documentTypeIdPtr) const
{
	QString filter = CreateFileDialogFilter(documentTypeIdPtr, iser::IFileLoader::QF_FILE | iser::IFileLoader::QF_LOAD);

	return QFileDialog::getOpenFileNames(Gui::GetWidget(), Gui::tr("Open Files..."), m_lastDirectory, filter);
}


} // namespace iqtdoc


#endif // !iqtdoc_TQtDocumentManagerWrap_included


