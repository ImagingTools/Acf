#include "iwin/CSystemInfo.h"

#include <winsock2.h>
#include <mbstring.h>
#include <iphlpapi.h>


namespace iwin
{


// public methods

CSystemInfo::CSystemInfo()
{
	GetSystemInfo();
}


// protected methods

void CSystemInfo::GetSystemInfo()
{
	char computerNameBuffer[1024] = {0};
	DWORD bufferSize = 1024;
	::GetComputerName(computerNameBuffer, &bufferSize);
	m_computerName = computerNameBuffer;

	SYSTEM_INFO info;
	::GetSystemInfo(&info);

	m_processorIds.resize(info.dwNumberOfProcessors);

	for (int processorIndex = 0; processorIndex < int(m_processorIds.size()); processorIndex++){
		m_processorIds[processorIndex] = info.wProcessorRevision;
	}

	m_networkAdapters = GetNetworkAdapters();
}


// private static methods

CSystemInfo::NetworkAdapters CSystemInfo::GetNetworkAdapters()
{
	NetworkAdapters adapters;

    IP_ADAPTER_INFO adapterInfo[100];
    DWORD dwBufLen = sizeof(adapterInfo);
    
	DWORD dwStatus = GetAdaptersInfo(adapterInfo, &dwBufLen);
	if(dwStatus != ERROR_SUCCESS){
        return adapters;
	}

    PIP_ADAPTER_INFO adapterInfoPtr = adapterInfo;
    char szBuffer[256];
    while(adapterInfoPtr != NULL){
        if(adapterInfoPtr->Type == MIB_IF_TYPE_ETHERNET){
			sprintf_s(szBuffer, sizeof(szBuffer), "%.2x-%.2x-%.2x-%.2x-%.2x-%.2x"
                        , adapterInfoPtr->Address[0]
                        , adapterInfoPtr->Address[1]
                        , adapterInfoPtr->Address[2]
                        , adapterInfoPtr->Address[3]
                        , adapterInfoPtr->Address[4]
                        , adapterInfoPtr->Address[5]);
			adapters.push_back(szBuffer);
        }
      
		adapterInfoPtr = adapterInfoPtr->Next;
    }

	return adapters;
}


} // namespace iwin

