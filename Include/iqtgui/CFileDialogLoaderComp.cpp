#include "iqtgui/CFileDialogLoaderComp.h"


#include <QObject>
#include <QAction>
#include <QFileDialog>


namespace iqtgui
{


// reimplemented (iser::IFileLoader)

bool CFileDialogLoaderComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const istd::CString* filePathPtr,
			int flags,
			bool beQuiet) const
{
	if (filePathPtr != NULL){
		const iser::IFileLoader* loaderPtr = GetLoaderFor(iqt::GetQString(*filePathPtr), -1, beQuiet);
		if (loaderPtr != NULL){
			return loaderPtr->IsOperationSupported(dataObjectPtr, filePathPtr, flags | QF_NAMED_ONLY, beQuiet);
		}
		return false;
	}

	if (m_loadersCompPtr.IsValid()){
		int loaderCount = m_loadersCompPtr.GetCount();
		for (int index = 0; index < loaderCount; index++){
			const iser::IFileLoader* slaveLoaderPtr = m_loadersCompPtr[index];
			if ((slaveLoaderPtr != NULL) && slaveLoaderPtr->IsOperationSupported(dataObjectPtr, filePathPtr, flags | QF_NAMED_ONLY)){
				return true;
			}
		}
	}

	if (!beQuiet){
		SendInfoMessage(MI_NONE_SUPPORTS, "None registered serializer supports specified type");
	}

	return false;
}


int CFileDialogLoaderComp::LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const
{
	int selectionIndex = -1;
	QString openFileName = GetFileName(filePath, false, selectionIndex);
	if (openFileName.isEmpty()){
		return StateAborted;
	}

	iser::IFileLoader* loaderPtr = GetLoaderFor(openFileName, selectionIndex, false);
	if (loaderPtr != NULL){
		return loaderPtr->LoadFromFile(data, iqt::GetCString(openFileName));
	}

	return StateFailed;
}


int CFileDialogLoaderComp::SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const
{
	int selectionIndex = -1;

	QString saveFileName = GetFileName(filePath, true, selectionIndex);
	if (saveFileName.isEmpty()){
		return StateAborted;
	}

	iser::IFileLoader* loaderPtr = GetLoaderFor(saveFileName, selectionIndex, false);
	if (loaderPtr != NULL){
		return loaderPtr->SaveToFile(data, iqt::GetCString(saveFileName));
	}

	return StateFailed;
}


bool CFileDialogLoaderComp::GetFileExtensions(istd::CStringList& result, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	int loadersCount = m_loadersCompPtr.GetCount();
	for (int i = 0; i < loadersCount; i++){
		const iser::IFileLoader* loaderPtr = m_loadersCompPtr[i];
		if (loaderPtr != NULL){
			loaderPtr->GetFileExtensions(result, true);
		}
	}

	return false;
}


istd::CString CFileDialogLoaderComp::GetTypeDescription(const istd::CString* /*extensionPtr*/) const
{
	return "";
}


// reimplemented (iser::IFileLoaderInfo)

istd::CString CFileDialogLoaderComp::GetLastLoadFileName() const
{
	return iqt::GetCString(m_lastOpenPath);
}


istd::CString CFileDialogLoaderComp::GetLastSaveFileName() const
{
	return iqt::GetCString(m_lastSavePath);
}


// static methods

int CFileDialogLoaderComp::AppendLoaderFilterList(const iser::IFileLoader& loader, QString& allExt, QString& result)
{
	int retVal = 0;

	istd::CStringList docExtensions;
	loader.GetFileExtensions(docExtensions);

	istd::CString commonDescription = loader.GetTypeDescription();

	if (!commonDescription.IsEmpty()){
		if (!result.isEmpty()){
			result += "\n";
		}

		result += iqt::GetQString(commonDescription + " (");
	}

	for (		istd::CStringList::const_iterator extIter = docExtensions.begin();
				extIter != docExtensions.end();
				++extIter){
		const istd::CString& extension = *extIter;

		if (!extension.IsEmpty()){
			if (commonDescription.IsEmpty()){
				istd::CString description = loader.GetTypeDescription(&extension);

				if (!result.isEmpty()){
					result += "\n";
				}

				result += iqt::GetQString(description + " (*." + extension + ")");

				++retVal;
			}
			else{
				if (extIter != docExtensions.begin()){
					result += ";";
				}

				result += iqt::GetQString(istd::CString("*.") + extension);
			}

			if (!allExt.isEmpty()){
				allExt += ";";
			}

			allExt += "*." + iqt::GetQString(extension);
		}
	}

	if (!commonDescription.IsEmpty()){
		result += ")";

		++retVal;
	}

	return retVal;
}


// protected methods

QString CFileDialogLoaderComp::GetFileName(const istd::CString& filePath, bool isSaving, int& selectionIndex) const
{
	selectionIndex = -1;

	QString retVal = iqt::GetQString(filePath);
	if (retVal.isEmpty()){
		QString filterList;
		QString allExt;
		int filtersCount = 0;

		int loadersCount = m_loadersCompPtr.GetCount();
		for (int i = 0; i < loadersCount; ++i){
			iser::IFileLoader* loaderPtr = m_loadersCompPtr[i];
			if (loaderPtr != NULL){
				filtersCount += iqtgui::CFileDialogLoaderComp::AppendLoaderFilterList(*loaderPtr, allExt, filterList);
			}
		}

		if (filtersCount > 1){
			filterList += "\n";
			filterList += QObject::tr("All known file types (%1)").arg(allExt);
		}

		QString selectedFilter;
		if (isSaving){
			retVal = QFileDialog::getSaveFileName(
						NULL,
						QObject::tr("Enter file name"), 
						m_lastSavePath,
						filterList,
						&selectedFilter); 
		}
		else{
			retVal = QFileDialog::getOpenFileName(
						NULL,
						QObject::tr("Select a file to open"),
						m_lastOpenPath,
						filterList,
						&selectedFilter);
		}

		int selectedPos = filterList.indexOf(selectedFilter);
		filterList.truncate(selectedPos);
		selectionIndex = filterList.count('\n');
	}

	QFileInfo fileInfo(retVal);
	if (isSaving){
		m_lastSavePath = fileInfo.absolutePath();
	}
	else{
		m_lastOpenPath = fileInfo.absolutePath();
	}

	return retVal;
}


iser::IFileLoader* CFileDialogLoaderComp::GetLoaderFor(const QString& filePath, int selectionIndex, bool beQuiet) const
{
	iser::IFileLoader* retVal = NULL;

	QFileInfo fileInfo(filePath);
	QString fileExtension = QString("*.") + fileInfo.suffix();

	int filtersSum = 0;
	int loadersCount = m_loadersCompPtr.GetCount();
	for (int i = 0; i < loadersCount; i++){
		iser::IFileLoader* loaderPtr = m_loadersCompPtr[i];
		if (loaderPtr != NULL){
			QString extensions;
			QString filters;
			int filtersCount = AppendLoaderFilterList(*loaderPtr, extensions, filters);
		
			if (extensions.contains(fileExtension)){
				if ((selectionIndex < 0) || ((selectionIndex >= filtersSum) && (selectionIndex < filtersSum + filtersCount))){
					return loaderPtr;
				}

				retVal = loaderPtr;
			}

			filtersSum += filtersCount;
		}
	}

	if (!beQuiet){
		SendInfoMessage(MI_NONE_SUPPORTS, "None registered serializer supports specified file");
	}

	return retVal;
}


} // namespace iqtgui


