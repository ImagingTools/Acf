/********************************************************************************
**
**	Copyright (C) 2007-2017 Witold Gantzke & Kirill Lepskiy
**
**	This file is part of the ACF Toolkit.
**
**	This file may be used under the terms of the GNU Lesser
**	General Public License version 2.1 as published by the Free Software
**	Foundation and appearing in the file LicenseLGPL.txt included in the
**	packaging of this file.  Please review the following information to
**	ensure the GNU Lesser General Public License version 2.1 requirements
**	will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**	If you are unsure which license is appropriate for your use, please
**	contact us at info@imagingtools.de.
**
** 	See http://www.ilena.org or write info@imagingtools.de for further
** 	information about the ACF.
**
********************************************************************************/


#include <ifile/CJsonFileWriteArchive.h>


// Qt includes
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>


namespace ifile
{


// public methods

CJsonFileWriteArchive::CJsonFileWriteArchive(const QString& filePath, const iser::IVersionInfo* infoPtr)
		: CJsonWriteArchiveBase()
{
	OpenFile(filePath);
}

CJsonFileWriteArchive::~CJsonFileWriteArchive()
{
	if (m_file.isOpen()){
		EndTag(m_rootTag);

		m_file.close();
	}
}


bool CJsonFileWriteArchive::OpenFile(const QString &filePath)
{
	m_file.setFileName(filePath);

	if (m_file.open(QIODevice::WriteOnly)){
		InitArchive(&m_file);
		return true;
	}

	return false;
}


} // namespace ifile


