// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QBuffer>

// ACF includes
#include <iser/CJsonReadArchiveBase.h>


namespace iser
{


class CJsonMemReadArchive : public iser::CJsonReadArchiveBase
{
public:
	typedef CJsonReadArchiveBase BaseClass;

	CJsonMemReadArchive(const QByteArray& data, bool serializeHeader = true);
	~CJsonMemReadArchive();
private:
	QBuffer m_buffer;
};


} // namespace iser


