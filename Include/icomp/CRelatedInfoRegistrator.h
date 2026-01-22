// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
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




