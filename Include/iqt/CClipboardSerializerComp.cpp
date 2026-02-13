// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iqt/CClipboardSerializerComp.h>


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QBuffer>
#include <QtCore/QMimeData>
#include <QtGui/QClipboard>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif

// ACF includes
#include <istd/CChangeNotifier.h>

#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>


namespace iqt
{


// reimplemented (ifile::IFilePersistence)

bool CClipboardSerializerComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const QString* filePathPtr,
			int flags,
			bool beQuiet) const
{
	if ((dataObjectPtr != NULL) && (dynamic_cast<const iser::ISerializable*>(dataObjectPtr) == NULL)){
		if (!beQuiet){
			SendInfoMessage(MI_BAD_OBJECT_TYPE, "Object is not serializable");
		}

		return false;
	}

	if ((flags & QF_ANONYMOUS) == 0){
		return false;
	}

	if ((flags & (QF_LOAD | QF_SAVE)) == 0){
		return false;
	}

	return (filePathPtr == NULL) || filePathPtr->isEmpty();
}


ifile::IFilePersistence::OperationState CClipboardSerializerComp::LoadFromFile(
			istd::IChangeable& data,
			const QString& /*filePath*/,
			ibase::IProgressManager* progressManagerPtr) const
{
	if (IsOperationSupported(&data, NULL, QF_LOAD | QF_ANONYMOUS, false)){
		const QClipboard* clipboardPtr = QApplication::clipboard();
		const QMimeData* mimeDataPtr = clipboardPtr->mimeData();

		QString mimeType = *m_mimeTypeAttrPtr;
		if (mimeDataPtr->hasFormat(mimeType)){
			QByteArray mimeData = mimeDataPtr->data(mimeType);
			if (!mimeData.isEmpty()){
				// Use device-based implementation with QBuffer
				QBuffer buffer(&mimeData);
				int result = ReadFromDevice(data, buffer, progressManagerPtr);
				
				if (result == ifile::IDeviceBasedPersistence::Successful){
					return OS_OK;
				}
				else{
					SendErrorMessage(MI_CANNOT_LOAD, "Cannot read data from clipboard archive (data corrupted)");
				}
			}
		}
		else{
			SendInfoMessage(MI_BAD_OBJECT_TYPE, "No MIME format in archive");
		}
	}

	return OS_FAILED;
}


ifile::IFilePersistence::OperationState CClipboardSerializerComp::SaveToFile(
			const istd::IChangeable& data, 
			const QString& /*filePath*/,
			ibase::IProgressManager* progressManagerPtr) const
{
	if (IsOperationSupported(&data, NULL, QF_SAVE | QF_ANONYMOUS, false)){
		QClipboard* clipboardPtr = QApplication::clipboard();
		if (clipboardPtr == NULL){
			return OS_CANCELED;
		}

		istd::TDelPtr<QMimeData> mimeDataPtr(new QMimeData);
		if (mimeDataPtr.IsValid()){
			// Use device-based implementation with QBuffer
			QByteArray byteArray;
			QBuffer buffer(&byteArray);
			
			int result = WriteToDevice(data, buffer, progressManagerPtr);
			
			if (result == ifile::IDeviceBasedPersistence::Successful){
				QString mimeType = *m_mimeTypeAttrPtr;
				mimeDataPtr->setData(mimeType, byteArray);

				clipboardPtr->setMimeData(mimeDataPtr.PopPtr());

				return OS_OK;
			}
			else{
				SendErrorMessage(MI_CANNOT_LOAD, "Cannot store data to archive");
			}
		}
	}

	return OS_FAILED;
}


// reimplemented (ifile::IFileTypeInfo)

bool CClipboardSerializerComp::GetFileExtensions(QStringList& result, const istd::IChangeable* /*dataObjectPtr*/, int /*flags*/, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	return true;
}


QString CClipboardSerializerComp::GetTypeDescription(const QString* /*extensionPtr*/) const
{
	return QString();
}


// reimplemented (ifile::IDeviceBasedPersistence)

bool CClipboardSerializerComp::IsDeviceOperationSupported(
			const istd::IChangeable& dataObject,
			const QIODevice& /*device*/,
			int /*deviceOperation*/) const
{
	return (dynamic_cast<const iser::ISerializable*>(&dataObject) != nullptr);
}


int CClipboardSerializerComp::ReadFromDevice(
			istd::IChangeable& data,
			QIODevice& device,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	iser::ISerializable* serializablePtr = dynamic_cast<iser::ISerializable*>(&data);
	if (serializablePtr == nullptr){
		return ifile::IDeviceBasedPersistence::Failed;
	}

	// Read all data from device into QByteArray
	QByteArray byteArray = device.readAll();
	
	// Use memory archive to deserialize
	iser::CMemoryReadArchive readArchive(byteArray);
	
	if (serializablePtr->Serialize(readArchive)){
		return ifile::IDeviceBasedPersistence::Successful;
	}

	return ifile::IDeviceBasedPersistence::Failed;
}


int CClipboardSerializerComp::WriteToDevice(
			const istd::IChangeable& data,
			QIODevice& device,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	const iser::ISerializable* serializablePtr = dynamic_cast<const iser::ISerializable*>(&data);
	if (serializablePtr == nullptr){
		return ifile::IDeviceBasedPersistence::Failed;
	}

	// Create memory archive to serialize
	iser::CMemoryWriteArchive writeArchive(m_versionInfoCompPtr.GetPtr());
	
	if (!(const_cast<iser::ISerializable*>(serializablePtr))->Serialize(writeArchive)){
		return ifile::IDeviceBasedPersistence::Failed;
	}

	// Write serialized data to device
	QByteArray byteArray = writeArchive.GetByteArray();
	qint64 written = device.write(byteArray);
	
	if (written == byteArray.size()){
		return ifile::IDeviceBasedPersistence::Successful;
	}

	return ifile::IDeviceBasedPersistence::Failed;
}


} // namespace iqt


