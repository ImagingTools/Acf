// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifile/CFileArchiveInfo.h>


namespace ifile
{


CFileArchiveInfo::CFileArchiveInfo(const QString& filePath)
:	m_filePath(filePath)
{
}


// reimplemented (ifile::IFileArchiveInfo)

const QString& CFileArchiveInfo::GetCurrentFilePath() const
{
	return m_filePath;
}


} // namespace ifile


