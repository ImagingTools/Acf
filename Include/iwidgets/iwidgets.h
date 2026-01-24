// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <iqt/iqt.h>


class QLayout;


/**
	Namespace contains implementation of Qt widget extensions
*/
namespace iwidgets
{


/**
	Remove all items (widgets and sub-layouts) from a given layout.
*/
void ClearLayout(QLayout* layout);


} // namespace iwidgets




