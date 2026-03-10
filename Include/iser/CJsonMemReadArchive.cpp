// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iser/CJsonMemReadArchive.h>


namespace iser
{


// public methods

CJsonMemReadArchive::CJsonMemReadArchive(const QByteArray& data, bool serializeHeader)
	:BaseClass(serializeHeader)
{
	m_buffer.setData(data);
	m_buffer.open(QIODevice::ReadOnly);

	SetContent(&m_buffer);
}


CJsonMemReadArchive::~CJsonMemReadArchive()
{
	m_buffer.close();
}


} // namespace iser


