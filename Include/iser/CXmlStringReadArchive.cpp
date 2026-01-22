// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iser/CXmlStringReadArchive.h>


namespace iser
{


CXmlStringReadArchive::CXmlStringReadArchive(
			const QByteArray& inputString,
			bool serializeHeader,
			const CArchiveTag& rootTag)
:	BaseClass(rootTag)
{
	m_buffer.setData(inputString);

	if (m_buffer.open(QIODevice::ReadOnly | QIODevice::Text)){
		m_stream.setDevice(&m_buffer);

		ReadXmlHeader();

		if (serializeHeader){
			SerializeAcfHeader();
		}
	}
}


} // namespace iser


