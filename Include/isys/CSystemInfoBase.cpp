#include "isys/CSystemInfoBase.h"


namespace isys
{


// public methods

// reimplemented (isys::ISystemInfo)

istd::CString CSystemInfoBase::GetComputerName() const
{
	return m_computerName;
}


int CSystemInfoBase::GetProcessorsCount() const
{
	return m_processorIds.size();
}


int CSystemInfoBase::GetProcessorId(int processorIndex) const
{
	I_ASSERT(processorIndex < int(m_processorIds.size()));
	I_ASSERT(processorIndex >= 0);

	return m_processorIds[processorIndex];
}


bool CSystemInfoBase::IsNetworkAdapterInstalled(int* adapterCountPtr) const
{
	if (adapterCountPtr != NULL){
		*adapterCountPtr = int(m_networkAdapters.size());
	}

	return (!m_networkAdapters.empty());
}


std::string CSystemInfoBase::GetMacAdress(int adapterIndex) const
{
	I_ASSERT(adapterIndex < int(m_networkAdapters.size()));
	I_ASSERT(adapterIndex >= 0);

	return m_networkAdapters[adapterIndex];
}


// reimplemented (iser::ISerializable)

bool CSystemInfoBase::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	return true;
}


} // namespace isys


