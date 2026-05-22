// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icomp/CCachedElementStaticInfo.h>


namespace icomp
{


// public methods

CCachedElementStaticInfo::CCachedElementStaticInfo()
{
}


void CCachedElementStaticInfo::AddInterfaceId(const QByteArray& interfaceId)
{
	m_interfaceIds.insert(interfaceId);
}


void CCachedElementStaticInfo::RegisterSubelementInfo(const QByteArray& subelementId, CCachedElementStaticInfo* subelementInfoPtr)
{
	m_subelementInfos[subelementId].SetPtr(subelementInfoPtr);
}


// reimplemented (icomp::IElementStaticInfo)

IElementStaticInfo::Ids CCachedElementStaticInfo::GetMetaIds(int metaGroupId) const
{
	if (metaGroupId == MGI_INTERFACES){
		return m_interfaceIds;
	}
	else if (metaGroupId == MGI_SUBELEMENTS){
		Ids retVal;
		for (		SubelementInfos::const_iterator iter = m_subelementInfos.begin();
					iter != m_subelementInfos.end();
					++iter){
			retVal.insert(iter.key());
		}
		return retVal;
	}

	return Ids();
}


const IElementStaticInfo* CCachedElementStaticInfo::GetSubelementInfo(const QByteArray& subcomponentId) const
{
	SubelementInfos::ConstIterator foundIter = m_subelementInfos.constFind(subcomponentId);
	if (foundIter != m_subelementInfos.constEnd()){
		return foundIter.value().GetPtr();
	}

	return NULL;
}


} // namespace icomp


