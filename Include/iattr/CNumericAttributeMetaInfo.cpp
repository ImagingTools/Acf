#include "iattr/CNumericAttributeMetaInfo.h"


namespace iattr
{


bool CNumericAttributeMetaInfo::SetMinAttributeValue(const iser::IObject* valuePtr, bool releaseFlag)
{
	m_minAttributePtr.SetPtr(valuePtr, releaseFlag);

	return true;
}


bool CNumericAttributeMetaInfo::SetMaxAttributeValue(const iser::IObject* valuePtr, bool releaseFlag)
{
	m_minAttributePtr.SetPtr(valuePtr, releaseFlag);

	return true;
}


// reimplemented (iattr::IEnumAttributeMetaInfo)

bool CNumericAttributeMetaInfo::GetMinimalValue(iser::IObject& result) const
{
	if (m_minAttributePtr.IsValid()){
		return result.CopyFrom(*m_minAttributePtr);
	}

	return false;
}


bool CNumericAttributeMetaInfo::GetMaximalValue(iser::IObject& result) const
{
	if (m_maxAttributePtr.IsValid()){
		return result.CopyFrom(*m_maxAttributePtr);
	}

	return false;
}


} // namespace iattr


