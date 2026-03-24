// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt inclides
#include <QtCore/QFile>

// ACF includes
#include <iser/CJsonReadArchiveBase.h>


namespace ifile
{


class CJsonFileReadArchive: public iser::CJsonReadArchiveBase
{
public:
	typedef iser::CJsonReadArchiveBase BaseClass;

	CJsonFileReadArchive(
				const QString& filePath = "",
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);

	bool OpenFile(const QString& filePath);

private:
	QFile m_file;
};


} // namespace ifile

