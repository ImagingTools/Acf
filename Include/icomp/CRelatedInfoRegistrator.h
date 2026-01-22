// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <icomp/IRealAttributeStaticInfo.h>


namespace icomp
{


class CRelatedInfoRegistrator
{
public:
	CRelatedInfoRegistrator(
				IRealAttributeStaticInfo& baseAttributeInfo,
				int metaGroupId,
				const QByteArray& id,
				int flags);
};


} // namespace icomp




