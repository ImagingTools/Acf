// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QDir>
#include <QtCore/QFile>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/CSystem.h>
#include <ibase/IProgressManager.h>
#include <ifile/CFileSerializerCompBase.h>


namespace ifile
{


/**
	Template implementation of file serializer using loading and storing archive implementation.

	\ingroup Persistence
*/
template <class ReadArchive, class WriteArchive>
class TFileSerializerComp: public CFileSerializerCompBase
{
public:	
	typedef CFileSerializerCompBase BaseClass;

	I_BEGIN_COMPONENT(TFileSerializerComp);
		I_ASSIGN(m_autoCreateDirectoryAttrPtr, "AutoCreatePath", "Create directory/file path automatically if not exists", true, false);
	I_END_COMPONENT;

	// reimplemented (ifile::IFilePersistence)
	virtual ifile::IFilePersistence::OperationState LoadFromFile(
				istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const override;
	virtual ifile::IFilePersistence::OperationState SaveToFile(
				const istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const override;

	// reimplemented (ifile::IDeviceBasedPersistence)
	virtual int ReadFromDevice(
				istd::IChangeable& data,
				QIODevice& device,
				ibase::IProgressManager* progressManagerPtr = nullptr) const override;
	virtual int WriteToDevice(
				const istd::IChangeable& data,
				QIODevice& device,
				ibase::IProgressManager* progressManagerPtr = nullptr) const override;

	// Device-based archive wrappers (work with any QIODevice including QFile)
	// Inherit directly from ReadArchive/WriteArchive to support both binary and text archives
	class ReadDeviceArchiveEx: public ReadArchive
	{
	public:
		typedef ReadArchive BaseClass;

		ReadDeviceArchiveEx(QIODevice& device, const istd::ILogger* loggerPtr)
		:	BaseClass(device),
			m_loggerPtr(loggerPtr)
		{
		}

		virtual bool SendLogMessage(
					istd::IInformationProvider::InformationCategory category,
					int id,
					const QString& message,
					const QString& messageSource,
					int flags = 0) const override
		{
			if (m_loggerPtr != nullptr){
				QString correctedMessage = message;
				QString correctedMessageSource = messageSource;

				BaseClass::DecorateMessage(category, id, flags, correctedMessage, correctedMessageSource);

				return m_loggerPtr->SendLogMessage(istd::IInformationProvider::IC_INFO, id, correctedMessage, correctedMessageSource, flags);
			}

			return false;
		}

		// reimplemented (istd::ILogger)
		virtual bool IsLogConsumed(
					const istd::IInformationProvider::InformationCategory* /*categoryPtr*/,
					const int* flagsPtr = nullptr) const override
		{
			static const istd::IInformationProvider::InformationCategory slaveCategory = istd::IInformationProvider::IC_INFO;

			return (m_loggerPtr != nullptr) && m_loggerPtr->IsLogConsumed(&slaveCategory, flagsPtr);
		}

	protected:

	private:
		const istd::ILogger* m_loggerPtr;
	};

	class WriteDeviceArchiveEx: public WriteArchive
	{
	public:
		typedef WriteArchive BaseClass;

		WriteDeviceArchiveEx(QIODevice& device, const iser::IVersionInfo* infoPtr, const istd::ILogger* loggerPtr)
		:	BaseClass(device, infoPtr),
			m_loggerPtr(loggerPtr)
		{
		}

		virtual bool SendLogMessage(
					istd::IInformationProvider::InformationCategory category,
					int id,
					const QString& message,
					const QString& messageSource,
					int flags = 0) const override
		{
			if (m_loggerPtr != nullptr){
				QString correctedMessage = message;
				QString correctedMessageSource = messageSource;

				BaseClass::DecorateMessage(category, id, flags, correctedMessage, correctedMessageSource);

				return m_loggerPtr->SendLogMessage(category, id, correctedMessage, correctedMessageSource, flags);
			}

			return false;
		}

		// reimplemented (istd::ILogger)
		virtual bool IsLogConsumed(
					const istd::IInformationProvider::InformationCategory* /*categoryPtr*/,
					const int* flagsPtr = nullptr) const override
		{
			static const istd::IInformationProvider::InformationCategory slaveCategory = istd::IInformationProvider::IC_INFO;

			return (m_loggerPtr != nullptr) && m_loggerPtr->IsLogConsumed(&slaveCategory, flagsPtr);
		}

	protected:

	private:
		const istd::ILogger* m_loggerPtr;
	};

protected:
	/**
		Called if read error is occurred.
	*/
	virtual void OnReadError(const ReadArchive& archive, const istd::IChangeable& data, const QString& filePath) const;

	I_ATTR(bool, m_autoCreateDirectoryAttrPtr);
};


// public methods

// reimplemented (ifile::IFilePersistence)

template <class ReadArchive, class WriteArchive>
ifile::IFilePersistence::OperationState TFileSerializerComp<ReadArchive, WriteArchive>::LoadFromFile(
			istd::IChangeable& data,
			const QString& filePath,
			ibase::IProgressManager* progressManagerPtr) const
{
	if (IsOperationSupported(&data, &filePath, QF_LOAD | QF_FILE, *m_beQuiteOnLoadAttrPtr)){
		// Use device-based implementation with QFile
		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly)){
			SendWarningMessage(MI_CANNOT_LOAD, QString(QObject::tr("Cannot open file for reading: ")) + filePath);
			return OS_FAILED;
		}
		
		int result = ReadFromDevice(data, file, progressManagerPtr);
		
		return (result == IDeviceBasedPersistence::Successful) ? OS_OK : OS_FAILED;
	}

	return OS_FAILED;
}


template <class ReadArchive, class WriteArchive>
ifile::IFilePersistence::OperationState TFileSerializerComp<ReadArchive, WriteArchive>::SaveToFile(
			const istd::IChangeable& data,
			const QString& filePath,
			ibase::IProgressManager* progressManagerPtr) const
{
	if (*m_autoCreateDirectoryAttrPtr){
		QFileInfo fileInfo(filePath);

		if (!istd::CSystem::EnsurePathExists(fileInfo.dir().absolutePath())){
			SendErrorMessage(MI_FILE_NOT_EXIST, QObject::tr("Cannot create path to file"));
			return OS_FAILED;
		}
	}

	if (IsOperationSupported(&data, &filePath, QF_SAVE | QF_FILE, false)){
		// Use device-based implementation with QFile
		QFile file(filePath);
		if (!file.open(QIODevice::WriteOnly)){
			SendInfoMessage(MI_CANNOT_SAVE, QObject::tr("Cannot open file for writing: '%1'").arg(filePath));
			return OS_FAILED;
		}
		
		int result = WriteToDevice(data, file, progressManagerPtr);
		
		if (result == IDeviceBasedPersistence::Successful){
			return OS_OK;
		}
		else{
			SendInfoMessage(MI_CANNOT_SAVE, QObject::tr("Cannot serialize object to file: '%1'").arg(filePath));
		}
	}

	return OS_FAILED;
}


// protected methods

template <class ReadArchive, class WriteArchive>
void TFileSerializerComp<ReadArchive, WriteArchive>::OnReadError(
			const ReadArchive& /*archive*/,
			const istd::IChangeable& /*data*/,
			const QString& filePath) const
{
	SendWarningMessage(MI_CANNOT_LOAD, QString(QObject::tr("Cannot load object from file ")) + filePath);
}


// reimplemented (ifile::IDeviceBasedPersistence)

template <class ReadArchive, class WriteArchive>
int TFileSerializerComp<ReadArchive, WriteArchive>::ReadFromDevice(
			istd::IChangeable& data,
			QIODevice& device,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	ReadDeviceArchiveEx archive(device, this);

	Q_ASSERT(!archive.IsStoring());

	iser::ISerializable* serializablePtr = dynamic_cast<iser::ISerializable*>(&data);
	if (serializablePtr == nullptr){
		serializablePtr = CompCastPtr<iser::ISerializable>(&data);
	}

	Q_ASSERT(serializablePtr != nullptr);

	if (serializablePtr->Serialize(archive)){
		return IDeviceBasedPersistence::Successful;
	}
	else{
		SendWarningMessage(IDeviceBasedPersistence::ReadOperationFailed, QObject::tr("Cannot load object from device"));
	}

	return IDeviceBasedPersistence::Failed;
}


template <class ReadArchive, class WriteArchive>
int TFileSerializerComp<ReadArchive, WriteArchive>::WriteToDevice(
			const istd::IChangeable& data,
			QIODevice& device,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	WriteDeviceArchiveEx archive(device, GetVersionInfo(), this);
	Q_ASSERT(archive.IsStoring());

	const iser::ISerializable* serializablePtr = dynamic_cast<const iser::ISerializable*>(&data);
	if(serializablePtr == nullptr){
		serializablePtr = CompCastPtr<iser::ISerializable>(&data);
	}
	Q_ASSERT(serializablePtr != nullptr);

	if (!CheckMinimalVersion(*serializablePtr, archive.GetVersionInfo())){
		SendWarningMessage(IDeviceBasedPersistence::UnsupportedArchiveVersion, QObject::tr("Archive version is not supported, possible lost of data"));
	}

	if ((const_cast<iser::ISerializable*>(serializablePtr))->Serialize(archive)){
		return IDeviceBasedPersistence::Successful;
	}
	else{
		SendInfoMessage(IDeviceBasedPersistence::WriteOperationFailed, QObject::tr("Cannot serialize object to device"));
	}

	return IDeviceBasedPersistence::Failed;
}


} // namespace ifile


