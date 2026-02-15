// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <idoc/CTextFileLoaderComp.h>


// Qt includes
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

// ACF includes
#include <idoc/ITextDocument.h>


namespace idoc
{


// reimplemented (ifile::IFilePersistence)

bool CTextFileLoaderComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const QString* filePathPtr,
			int flags,
			bool /*beQuiet*/) const
{
	if ((filePathPtr != NULL) && filePathPtr->isEmpty()){
		return false;
	}

	return		((dataObjectPtr == NULL) || (dynamic_cast<const idoc::ITextDocument*>(dataObjectPtr) != NULL)) &&
				((flags & (QF_LOAD | QF_SAVE)) != 0) &&
				((flags & QF_FILE) != 0);
}


ifile::IFilePersistence::OperationState CTextFileLoaderComp::LoadFromFile(
			istd::IChangeable& data,
			const QString& filePath,
			ibase::IProgressManager* progressManagerPtr) const
{
	if (!IsOperationSupported(&data, &filePath, QF_LOAD | QF_FILE, false)){
		return OS_FAILED;
	}

	// Use device-based implementation with QFile
	QFile file(filePath);
	int result = ReadFromDevice(data, file, progressManagerPtr);
	
	return (result == ifile::IDeviceBasedPersistence::Successful) ? OS_OK : OS_FAILED;
}


ifile::IFilePersistence::OperationState CTextFileLoaderComp::SaveToFile(
			const istd::IChangeable& data,
			const QString& filePath,
			ibase::IProgressManager* progressManagerPtr) const
{
	if (!IsOperationSupported(&data, &filePath, QF_SAVE | QF_FILE, false)){
		return OS_FAILED;
	}

	// Use device-based implementation with QFile
	QFile file(filePath);
	int result = WriteToDevice(data, file, progressManagerPtr);
	
	return (result == ifile::IDeviceBasedPersistence::Successful) ? OS_OK : OS_FAILED;
}


// reimplemented (ifile::IDeviceBasedPersistence)

bool CTextFileLoaderComp::IsDeviceOperationSupported(
			const istd::IChangeable& dataObject,
			const QIODevice& /*device*/,
			int /*deviceOperation*/) const
{
	return (dynamic_cast<const idoc::ITextDocument*>(&dataObject) != nullptr);
}


int CTextFileLoaderComp::ReadFromDevice(
			istd::IChangeable& data,
			QIODevice& device,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	idoc::ITextDocument* documentPtr = dynamic_cast<idoc::ITextDocument*>(&data);
	if (documentPtr == nullptr){
		return ifile::IDeviceBasedPersistence::Failed;
	}

	// Ensure device is open for reading
	if (!device.isOpen()){
		if (!device.open(QIODevice::ReadOnly | QIODevice::Text)){
			return ifile::IDeviceBasedPersistence::Failed;
		}
	}
	else if (!device.isReadable()){
		// Device is open but not readable
		return ifile::IDeviceBasedPersistence::Failed;
	}

	QTextStream stream(&device);
	QString documentText = stream.readAll();
	
	documentPtr->SetText(documentText);

	return ifile::IDeviceBasedPersistence::Successful;
}


int CTextFileLoaderComp::WriteToDevice(
			const istd::IChangeable& data,
			QIODevice& device,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	const idoc::ITextDocument* documentPtr = dynamic_cast<const idoc::ITextDocument*>(&data);
	if (documentPtr == nullptr){
		return ifile::IDeviceBasedPersistence::Failed;
	}

	// Ensure device is open for writing
	if (!device.isOpen()){
		if (!device.open(QIODevice::WriteOnly | QIODevice::Text)){
			return ifile::IDeviceBasedPersistence::Failed;
		}
	}
	else if (!device.isWritable()){
		// Device is open but not writable
		return ifile::IDeviceBasedPersistence::Failed;
	}

	QTextStream stream(&device);
	stream << documentPtr->GetText();

	return ifile::IDeviceBasedPersistence::Successful;
}


// reimplemented (ifile::IFileTypeInfo)

bool CTextFileLoaderComp::GetFileExtensions(QStringList& result, const istd::IChangeable* /*dataObjectPtr*/, int /*flags*/, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	int extensionsCount = qMin(m_fileExtensionsAttrPtr.GetCount(), m_typeDescriptionsAttrPtr.GetCount());
	for (int extIndex = 0; extIndex < extensionsCount; extIndex++){
		result.push_back(m_fileExtensionsAttrPtr[extIndex]);
	}

	return true;
}


QString CTextFileLoaderComp::GetTypeDescription(const QString* extensionPtr) const
{
	if (extensionPtr != NULL){
		int extensionsCount = qMin(m_fileExtensionsAttrPtr.GetCount(), m_typeDescriptionsAttrPtr.GetCount());
		for (int extIndex = 0; extIndex < extensionsCount; extIndex++){
			if (m_fileExtensionsAttrPtr[extIndex] == *extensionPtr){
				return m_typeDescriptionsAttrPtr[extIndex];
			}
		}
	}

	if (m_typeDescriptionsAttrPtr.GetCount() > 0){
		return m_typeDescriptionsAttrPtr[0];
	}

	return "";
}


} // namespace idoc


