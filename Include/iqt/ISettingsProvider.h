// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QSettings>

// ACF includes
#include <istd/IPolymorphic.h>
#include <iqt/iqt.h>


namespace iqt
{


/**
	Interface for access the application settings object.
*/
class ISettingsProvider: virtual public istd::IPolymorphic
{
public:
	/**
		Get the instance of the Qt settings.
	*/
	virtual QSettings& GetSettings() const = 0;
};


} // namespace iqt




