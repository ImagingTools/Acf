// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QFile>
#include <QtCore/QTextStream>

// ACF includes
#include <ifile/IFileNameParam.h>
#include <ifile/IFilePersistence.h>
#include <ifile/IDeviceBasedPersistence.h>
#include <ilog/CStreamLogCompBase.h>


namespace ifile
{


/**
	Message container displaying messages as log list.
*/
class CTextFileLogStreamerComp: 
			public ilog::CStreamLogCompBase,
			public ifile::IFilePersistence,
			public ifile::IDeviceBasedPersistence
{
public:
	typedef ilog::CStreamLogCompBase BaseClass;

	I_BEGIN_COMPONENT(CTextFileLogStreamerComp);
		I_REGISTER_INTERFACE(ifile::IFilePersistence);
		I_REGISTER_INTERFACE(ifile::IDeviceBasedPersistence);
		I_ASSIGN(m_isAppendAttrPtr, "AppendToExisting", "Don't overwrite existing log file", true, false);
	I_END_COMPONENT;

	CTextFileLogStreamerComp();

	// reimplemented (ifile::IFilePersistence)
	virtual bool IsOperationSupported(
		const istd::IChangeable* dataObjectPtr,
		const QString* filePathPtr = NULL,
		int flags = -1,
		bool beQuiet = true) const override;
	virtual ifile::IFilePersistence::OperationState LoadFromFile(
				istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const override;
	virtual ifile::IFilePersistence::OperationState SaveToFile(
				const istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const override;

	// reimplemented (ifile::IDeviceBasedPersistence)
	virtual bool IsDeviceOperationSupported(
				const istd::IChangeable& dataObject,
				const QIODevice& device,
				int deviceOperation) const override;
	virtual int ReadFromDevice(
				istd::IChangeable& data,
				QIODevice& device,
				ibase::IProgressManager* progressManagerPtr = nullptr) const override;
	virtual int WriteToDevice(
				const istd::IChangeable& data,
				QIODevice& device,
				ibase::IProgressManager* progressManagerPtr = nullptr) const override;

	// reimplemented (ifile::IFileTypeInfo)
	virtual bool GetFileExtensions(QStringList& result, const istd::IChangeable* dataObjectPtr = NULL, int flags = -1, bool doAppend = false) const override;
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const override;

protected:
	// reimplemented (CStreamLogCompBase)
	virtual void WriteText(const QString& text, istd::IInformationProvider::InformationCategory category) override;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

private:
	bool OpenFileStream(const QString& filePath);
	void CloseFileStream();

private:
	I_ATTR(bool, m_isAppendAttrPtr);

	mutable QFile m_outputFile;
	mutable QTextStream m_outputFileStream;
};


} // namespace ifile


