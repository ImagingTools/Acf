// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtGui/QIcon>


// ACF includes
#include <istd/IChangeable.h>
#include <iqtgui/iqtgui.h>


namespace iqtgui
{


/**
	This interface provides acces to an icon.
*/
class IIconProvider: virtual public istd::IChangeable
{
public:
	/**
		Gets the number of icons in the collection.
	*/
	virtual int GetIconCount() const = 0;

	/**
		Gets icon with the index \c iconIndex from collection.
	*/
	virtual QIcon GetIcon(int iconIndex) const = 0;
};


} // namespace iqtgui



