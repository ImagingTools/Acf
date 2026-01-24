// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <ifile/IFileNameParam.h>


namespace ifile
{


class IRelativeFilePath: virtual public IFileNameParam
{
public:
	virtual QString GetBasePath() const = 0;
};


} // namespace ifile


