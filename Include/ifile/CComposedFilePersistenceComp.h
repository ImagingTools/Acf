// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <ifile/IFilePersistence.h>
#include <ifile/IDeviceBasedPersistence.h>
#include <ifile/IFilePersistenceInfo.h>
#include <icomp/CComponentBase.h>

// Qt includes
#include <QtCore/QFileInfo>


namespace ifile
{


/**
	Loader component, which delegates the persistence functionality to its slave loaders.

	\ingroup Persistence
*/
class CComposedFilePersistenceComp:
			public icomp::CComponentBase,
			virtual public ifile::IFilePersistence,
			virtual public ifile::IDeviceBasedPersistence,
			virtual public ifile::IFilePersistenceInfo
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CComposedFilePersistenceComp);
		I_REGISTER_INTERFACE(ifile::IFileTypeInfo);
		I_REGISTER_INTERFACE(ifile::IFilePersistence);
		I_REGISTER_INTERFACE(ifile::IDeviceBasedPersistence);
		I_REGISTER_INTERFACE(ifile::IFilePersistenceInfo);
		I_ASSIGN_MULTI_0(m_slaveLoadersCompPtr, "SlaveLoaders", "List of slave loaders", true);
		I_ASSIGN(m_commonDescriptionAttrPtr, "CommonDescription", "Optional common description of this file type", false, "Composed");
	I_END_COMPONENT;

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

	// reimplemented (ifile::IFilePersistenceInfo)
	virtual QString GetLastFilePath(OperationType operationType = OT_UNKNOWN, PathType pathType = PT_COMPLETE) const override;

protected:
	QString GetPathForType(const QFileInfo& fileInfo, PathType pathType) const;

private:
	I_MULTIREF(ifile::IFilePersistence, m_slaveLoadersCompPtr);
	I_TEXTATTR(m_commonDescriptionAttrPtr);

	mutable QFileInfo m_lastOpenInfo;
	mutable QFileInfo m_lastSaveInfo;
};


} // namespace ifile


