// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifile/CJsonFileReadArchive.h>


namespace ifile
{


// public methods

CJsonFileReadArchive::CJsonFileReadArchive(const QString& filePath, bool serializeHeader)
	:BaseClass(serializeHeader)
{
	OpenFile(filePath);
}


bool CJsonFileReadArchive::OpenFile(const QString &filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		return false;
	}

	if (!BaseClass::SetContent(&file)){
		file.close();

		return false;
	}

	return true;
}


} // namespace iser


