// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/TInterfacePtr.h>
#include <istd/IFactoryInfo.h>


namespace istd
{


template <class InterfaceType>
class TInterfaceFactory: virtual public istd::IFactoryInfo
{
public:
	virtual istd::TUniqueInterfacePtr<InterfaceType> CreateInstance(const QByteArray& typeId = QByteArray()) const = 0;
};


} // namespace istd


