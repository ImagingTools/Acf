// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QFileInfoList>

// ACF includes
#include <istd/IChangeable.h>


namespace ifile
{


/**
	Provide access to a list of files.
*/
class IFileListProvider: virtual public istd::IChangeable
{
public:
	/**
		Get list of files.
	*/
	virtual const QFileInfoList& GetFileList() const = 0;
};


} // namespace ifile


