#include "icbox/CMemoryBankSerializerComp.h"


// Windows API
#include <windows.h>

// CryptoBox API
#include "cbios.h"

#include "iser/CMemoryReadArchive.h"
#include "iser/CMemoryWriteArchive.h"


namespace icbox
{


CMemoryBankSerializerComp::CMemoryBankSerializerComp()
:	m_isInitialized(false),
	m_isOpened(false),
	m_isLogged(false)
{
}


// reimplemented (icomp::IComponent)

void CMemoryBankSerializerComp::OnComponentCreated()
{
	m_isInitialized = (::CBIOS_StartupDLL() != CBIOS_ERR_NOT_INITIALIZED);
}


void CMemoryBankSerializerComp::OnComponentDestroyed()
{
	EnsurePartitionClosed();

	if (m_isInitialized){
		::CBIOS_FinishDLL();

		m_isInitialized = false;
	}
}


// reimplemented (iser::IFileLoader)

bool CMemoryBankSerializerComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const istd::CString* filePathPtr,
			int flags,
			bool beQuiet) const
{
	if ((dataObjectPtr != NULL) && (dynamic_cast<const iser::ISerializable*>(dataObjectPtr) == NULL)){
		if (!beQuiet){
			SendInfoMessage(MI_BAD_OBJECT_TYPE, "Object is not serializable");
		}

		return false;
	}

	if ((flags & QF_NAMED_ONLY) != 0){
		return false;
	}

	if (		(*m_memoryBankIdAttrPtr != 3) &&
				!m_accessKeyAttrPtr.IsValid()){
		if (!beQuiet){
			SendInfoMessage(MI_BAD_OBJECT_TYPE, "User or administrator password is needed to access bank 1 and 2");
		}

		return false;
	}

	if (		((flags & QF_NO_LOADING) != 0) &&
				(*m_memoryBankIdAttrPtr == 2) &&
				(!m_accessKeyAttrPtr.IsValid() || !*m_isAdminKeyAttrPtr)){
		if (!beQuiet){
			SendInfoMessage(MI_BAD_OBJECT_TYPE, "Administrator password is needed to write bank 2");
		}

		return false;
	}

	return m_isOpened && ((filePathPtr == NULL) || filePathPtr->IsEmpty());
}


int CMemoryBankSerializerComp::LoadFromFile(istd::IChangeable& data, const istd::CString& /*filePath*/) const
{
	if (EnsurePartitionOpened() && IsOperationSupported(&data, NULL, QF_NO_SAVING | QF_ANONYMOUS_ONLY, false)){
		iser::ISerializable* serializablePtr = dynamic_cast<iser::ISerializable*>(&data);
		I_ASSERT(serializablePtr != NULL);	// it was checked in IsOperationSupported

		I_DWORD blockSize;
		if (!ReadFromMem(0, &blockSize, sizeof(blockSize)) || (blockSize < 0) || (blockSize > 0xffff)){
			return StateFailed;
		}

		std::vector<I_BYTE> buffer(blockSize, 0);
		if (ReadFromMem(sizeof(blockSize), &buffer[0], blockSize) || (blockSize > 0xffff)){
			iser::CMemoryReadArchive archive(&buffer, blockSize);

			if (serializablePtr->Serialize(archive)){
				return StateOk;
			}
			else{
				SendErrorMessage(MI_CANNOT_LOAD, "Cannot load data from Crypto Box, data corrupted");
			}

		}
	}

	return StateFailed;
}


int CMemoryBankSerializerComp::SaveToFile(const istd::IChangeable& data, const istd::CString& /*filePath*/) const
{
	if (EnsurePartitionOpened() && IsOperationSupported(&data, NULL, QF_NO_LOADING | QF_ANONYMOUS_ONLY, false)){
		iser::ISerializable* serializablePtr = dynamic_cast<iser::ISerializable*>(const_cast<istd::IChangeable*>(&data));
		I_ASSERT(serializablePtr != NULL);	// it was checked in IsOperationSupported

		iser::CMemoryWriteArchive archive(m_versionInfoCompPtr.GetPtr());
		if (!serializablePtr->Serialize(archive)){
			SendErrorMessage(MI_CANNOT_LOAD, "Cannot store data to archive");
			return StateFailed;
		}

		I_DWORD blockSize = I_DWORD(archive.GetBufferSize());
		if (WriteToMem(0, &blockSize, sizeof(blockSize)) && WriteToMem(sizeof(blockSize), archive.GetBuffer(), blockSize)){
			return StateOk;
		}
		else{
			SendErrorMessage(MI_CANNOT_LOAD, "Cannot write data to Crypto Box");
		}
	}

	return StateFailed;
}


bool CMemoryBankSerializerComp::GetFileExtensions(istd::CStringList& /*result*/, int /*flags*/, bool /*doAppend*/) const
{
	return false;
}


istd::CString CMemoryBankSerializerComp::GetTypeDescription(const istd::CString* /*extensionPtr*/) const
{
	return istd::CString();
}


// protected methods

bool CMemoryBankSerializerComp::CheckError(I_DWORD errorCode) const
{
	if (errorCode == 0){
		return true;
	}

	if (errorCode & CBIOS_WARNING_MASK){
		SendWarningMessage(MI_CBIOS_ERROR, istd::CString("CBIOS Warning code ") + istd::CString::FromNumber(errorCode));

		return true;
	}

	SendErrorMessage(MI_CBIOS_ERROR, istd::CString("CBIOS Error code ") + istd::CString::FromNumber(errorCode));

	return false;
}


bool CMemoryBankSerializerComp::EnsurePartitionOpened() const
{
	if (m_isOpened){
		return true;
	}

	if (!m_isInitialized){
		return false;
	}

	if (CheckError(::CBIOS_OpenByApp(I_WORD(*m_partitionIdAttrPtr)))){
		return false;
	}

	m_isOpened = true;

	if (!m_accessKeyAttrPtr.IsValid()){
		return false;
	}

	if (*m_memoryBankIdAttrPtr != 3){
		I_BYTE password[16] = {0};
		const std::string& passwordStr = (*m_accessKeyAttrPtr).ToString();
		::memcpy(password, passwordStr.data(), istd::Min(sizeof(password), passwordStr.size()));

		if (CheckError((*m_isAdminKeyAttrPtr)? ::CBIOS_APWLogin(password): ::CBIOS_UPWLogin(password))){
			return false;
		}

		m_isLogged = true;
	}

	return true;
}


bool CMemoryBankSerializerComp::EnsurePartitionClosed() const
{
	if (m_isLogged){
		::CBIOS_Logout();

		m_isLogged = false;
	}

	if (m_isOpened){
		::CBIOS_Close();

		m_isOpened = false;
	}

	return true;
}


bool CMemoryBankSerializerComp::ReadFromMem(int offset, void* bufferPtr, int size) const
{
	I_BYTE password[16] = {0};
	if (*m_memoryBankIdAttrPtr == 3){
		return !CheckError(::CBIOS_ReadRAM3(offset, size, bufferPtr, password));
	}
	else{
		const std::string& passwordStr = (*m_accessKeyAttrPtr).ToString();
		::memcpy(password, passwordStr.data(), istd::Min(sizeof(password), passwordStr.size()));

		if (*m_memoryBankIdAttrPtr == 1){
			return !CheckError(::CBIOS_ReadRAM1(offset, size, bufferPtr, password));
		}
		else if (*m_memoryBankIdAttrPtr == 2){
			return !CheckError(::CBIOS_ReadRAM2(offset, size, bufferPtr, password));
		}
	}

	return false;
}


bool CMemoryBankSerializerComp::WriteToMem(int offset, const void* bufferPtr, int size) const
{
	I_BYTE password[16] = {0};
	if (*m_memoryBankIdAttrPtr == 3){
		return !CheckError(::CBIOS_WriteRAM3(offset, size, (PVOID)bufferPtr, password));
	}
	else{
		const std::string& passwordStr = (*m_accessKeyAttrPtr).ToString();
		::memcpy(password, passwordStr.data(), istd::Min(sizeof(password), passwordStr.size()));

		if (*m_memoryBankIdAttrPtr == 1){
			return !CheckError(::CBIOS_WriteRAM1(offset, size, (PVOID)bufferPtr, password));
		}
		else if (*m_memoryBankIdAttrPtr == 2){
			return !CheckError(::CBIOS_WriteRAM2(offset, size, (PVOID)bufferPtr, password));
		}
	}

	return false;
}


} // namespace icbox


