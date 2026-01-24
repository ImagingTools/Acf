// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QMimeData>
#include <QtCore/QEvent>

// ACF includes
#include <istd/IPolymorphic.h>
#include <iqtgui/iqtgui.h>


namespace iqtgui
{


/**
	Interface for objects which can receive dragged objects
*/
class IDropConsumer: virtual public istd::IPolymorphic
{
public:
	/**
		Get the list of the possible mime IDs, which are accepted by the consumer.
	*/
	virtual QStringList GetAcceptedMimeIds() const = 0;

	/**
		Notification about drop action finish.
	*/
	virtual void OnDropFinished(const QMimeData& mimeData, QEvent* eventPtr) = 0;
};


} // namespace iqtgui




