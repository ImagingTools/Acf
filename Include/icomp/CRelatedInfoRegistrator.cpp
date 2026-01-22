// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icomp/CRelatedInfoRegistrator.h>


namespace icomp
{


CRelatedInfoRegistrator::CRelatedInfoRegistrator(
			IRealAttributeStaticInfo& baseAttributeInfo,
			int metaGroupId,
			const QByteArray& id,
			int flags)
{
	int baseFlags = baseAttributeInfo.GetAttributeFlags();
	if ((baseFlags & IAttributeStaticInfo::AF_NULLABLE) != 0){
		flags |= IAttributeStaticInfo::AF_NULLABLE;
	}
	baseAttributeInfo.AddRelatedMetaId(metaGroupId, id, flags);
}


} // namespace icomp


