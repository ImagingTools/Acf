// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifile/CJsonFileReadArchive.h>


namespace ifile
{


// public methods

CJsonFileReadArchive::CJsonFileReadArchive(
			const QString& filePath,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
	:BaseClass(serializeHeader, rootTag)
{
	OpenFile(filePath);
}


bool CJsonFileReadArchive::OpenFile(const QString &filePath)
{
	if (m_file.isOpen()){
		m_file.close();
	}

	m_file.setFileName(filePath);
	if (!m_file.open(QIODevice::ReadOnly | QIODevice::Text)){
		return false;
	}

	if (!BaseClass::SetContent(&m_file)){
		m_file.close();

		return false;
	}

	return true;
}


} // namespace ifile

