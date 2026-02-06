// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <ifile/CFileReadArchive.h>

// QSF includes
#include <ifile/CSimpleEncoder.h>


namespace ifile
{


class CFileReadSecureArchive:
			public ifile::CFileReadArchive,
			protected CSimpleEncoder
{
public:
	typedef ifile::CFileReadArchive BaseClass;

	explicit CFileReadSecureArchive(
				const QString& filePath,
				bool supportTagSkipping = true,
				bool serializeHeader = true);

	// reimplemented (ifile::CFileReadArchive)
	virtual bool ProcessData(void* data, int size) override;
};


} // namespace ifile


