#include "iqt/CXslSerializerComp.h"


namespace iqt
{


bool CXslSerializerComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const istd::CString* filePathPtr,
			int flags,
			bool beQuiet) const
{
	if ((dataObjectPtr != NULL) && (dynamic_cast<const iser::ISerializable*>(dataObjectPtr) == NULL)){
		if (!beQuiet){
			SendInfoMessage(MI_BAD_OBJECT_TYPE, tr("Object is not serializable"));
		}

		return false;
	}

	if ((flags & (QF_DIRECTORY_ONLY | QF_ANONYMOUS_ONLY)) != 0){
		return false;
	}

	if (filePathPtr != NULL){
		if ((flags & QF_NO_SAVING) != 0){
			isys::IFileSystem* fileSystemPtr = istd::GetService<isys::IFileSystem>();
			if (fileSystemPtr != NULL){
				if (!fileSystemPtr->IsPresent(*filePathPtr)){
					return false;
				}
			}
		}

		istd::CStringList fileExtensions;
		if (GetFileExtensions(fileExtensions)){
			int extensionsCount = fileExtensions.size();
			for (int i = 0; i < extensionsCount; ++i){
				const istd::CString& extension = fileExtensions[i];
				if (!filePathPtr->IsEmpty() && filePathPtr->substr(filePathPtr->length() - extension.length() - 1) == istd::CString(".") + extension.ToLower()){
					return true;
				}
			}

			if (!beQuiet){
				SendInfoMessage(MI_BAD_EXTENSION, tr("File extension is not supported"));
			}

			return false;
		}
	}

	return true;
}


int CXslSerializerComp::LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const
{
	if (!CheckInputFilePath(filePath)){
		return false;
	}

	if (IsOperationSupported(&data, &filePath, QF_NO_SAVING, false)){
		CXslTransformationReadArchive archive(filePath, m_xslReadFilePath.GetPtr()->GetPath());

		I_ASSERT(!archive.IsStoring());

		iser::ISerializable* serializablePtr = dynamic_cast<iser::ISerializable*>(&data);
		I_ASSERT(serializablePtr != NULL);

		istd::CChangeNotifier changePtr(NULL, istd::IChangeable::CF_MODEL);

		if (serializablePtr->Serialize(archive)){
			changePtr.SetPtr(&data);

			return StateOk;
		}
		else{
			OnReadError(archive, data, filePath);
		}
	}
	return StateFailed;
}


int CXslSerializerComp::SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const
{
	if (!CheckTargetDirectory(filePath)){
		return StateFailed;
	}

	if (IsOperationSupported(&data, &filePath, QF_NO_LOADING, false)){
		CXslTransformationWriteArchive archive(filePath, m_xslWriteFilePath.GetPtr()->GetPath(), GetVersionInfo(), this);
		I_ASSERT(archive.IsStoring());

		const iser::ISerializable* serializablePtr = dynamic_cast<const iser::ISerializable*>(&data);
		I_ASSERT(serializablePtr != NULL);

		if (!CheckMinimalVersion(*serializablePtr, archive.GetVersionInfo())){
			SendWarningMessage(MI_UNSUPPORTED_VERSION, tr("Archive version is not supported, possible lost of data"));
		}

		if ((const_cast<iser::ISerializable*>(serializablePtr))->Serialize(archive)){
			return StateOk;
		}
		else{
			SendInfoMessage(MI_CANNOT_SAVE, tr("Cannot serialize object to file"));
		}
	}

	return StateFailed;
}


// protected methods


const iser::IVersionInfo* CXslSerializerComp::GetVersionInfo() const
{
	return m_versionInfoCompPtr.GetPtr();
}


void CXslSerializerComp::OnReadError(const iser::IArchive& /*archive*/, const istd::IChangeable& /*data*/, const istd::CString& filePath) const
{
	SendWarningMessage(MI_CANNOT_LOAD, istd::CString(tr("Cannot load object from file ")) + filePath);
}


bool CXslSerializerComp::CheckMinimalVersion(const iser::ISerializable& object, const iser::IVersionInfo& versionInfo) const
{
	iser::IVersionInfo::VersionIds ids = versionInfo.GetVersionIds();

	for (		iser::IVersionInfo::VersionIds::const_iterator iter = ids.begin();
				iter != ids.end();
				++iter){
		int id = *iter;

		I_DWORD objectMinimalVersion = object.GetMinimalVersion(id);

		I_DWORD versionNumber;
		if (versionInfo.GetVersionNumber(id, versionNumber)){
			if (versionNumber < objectMinimalVersion){
				return false;
			}
		}
	}

	return true;
}


bool CXslSerializerComp::CheckInputFilePath(const istd::CString filePath) const
{
	isys::IFileSystem* fileSystemPtr = istd::GetService<isys::IFileSystem>();
	if (fileSystemPtr == NULL){
		SendWarningMessage(0, "File path could not be checked");

		return true;
	}
	
	if (!fileSystemPtr->IsPresent(filePath)){
		SendWarningMessage(MI_CANNOT_LOAD, "File " + filePath.ToString() + " does not exist");

		return false;
	}
	
	return true;
}


bool CXslSerializerComp::CheckTargetDirectory(const istd::CString dirPath) const
{
	isys::IFileSystem* fileSystemPtr = istd::GetService<isys::IFileSystem>();
	if (fileSystemPtr == NULL){
		SendWarningMessage(0, "Target directory path could not be checked");

		return true;
	}

	if (!fileSystemPtr->IsPresent(fileSystemPtr->GetDirPath(dirPath))){
		SendWarningMessage(MI_CANNOT_SAVE, "Save target directory " + fileSystemPtr->GetDirPath(dirPath).ToString() + " does not exist");

		return false;
	}

	return true;
}


} // namespace iqt


