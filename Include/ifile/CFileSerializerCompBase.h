// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QFileInfo>

// ACF includes
#include <ilog/TLoggerCompWrap.h>
#include <ifile/IFilePersistence.h>
#include <ifile/IDeviceBasedPersistence.h>
#include <ifile/CFileTypeInfoComp.h>


namespace ifile
{


/**
	Base implementation of file serializer.
	Serializer allows to load and save objects implementing iser::ISerializable interface.

	\ingroup Persistence
*/
class CFileSerializerCompBase:
			public ilog::TLoggerCompWrap<CFileTypeInfoComp>,
			virtual public ifile::IFilePersistence,
			virtual public ifile::IDeviceBasedPersistence
{
public:	
	typedef ilog::TLoggerCompWrap<CFileTypeInfoComp> BaseClass;

	I_BEGIN_BASE_COMPONENT(CFileSerializerCompBase);
		I_REGISTER_INTERFACE(ifile::IFileTypeInfo);
		I_REGISTER_INTERFACE(ifile::IFilePersistence);
		I_REGISTER_INTERFACE(ifile::IDeviceBasedPersistence);
		I_ASSIGN(m_beQuiteOnLoadAttrPtr, "BeQuiteOnLoad", "Do not log message when loading if file is missing", true, false);
		I_ASSIGN(m_versionInfoCompPtr, "VersionInfo", "Provide information about archive versions", false, "VersionInfo");
	I_END_COMPONENT;

	// reimplemented (ifile::IFilePersistence)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const QString* filePathPtr = nullptr,
				int flags = -1,
				bool beQuiet = true) const override;

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

protected:
	/**
		Get working version info.
	*/
	virtual const iser::IVersionInfo* GetVersionInfo() const;

	/**
		Check if the minimal version of some serializable object is supported by version info.
	*/
	bool CheckMinimalVersion(const iser::ISerializable& object, const iser::IVersionInfo& versionInfo) const;

	/**
		Check if input file is OK.
	*/
	bool CheckInputFile(const QString filePath, bool beQuiet = true) const;

	/**
		Check if output file is OK.
	*/
	bool CheckOutputFile(const QString filePath, bool beQuiet = true) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

protected:
	I_ATTR(bool, m_beQuiteOnLoadAttrPtr);

private:
	I_REF(iser::IVersionInfo, m_versionInfoCompPtr);
};


} // namespace ifile


