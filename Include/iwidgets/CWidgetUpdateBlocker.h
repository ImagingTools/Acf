// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif


namespace iwidgets
{


/**
	Help class used to block widget update till it is destructed.
*/
class CWidgetUpdateBlocker
{
public:
	explicit CWidgetUpdateBlocker(QWidget* widgetPtr);
	~CWidgetUpdateBlocker();

private:
	QWidget* m_widgetPtr;
};


// inline methods

inline CWidgetUpdateBlocker::CWidgetUpdateBlocker(QWidget* widgetPtr)
	:m_widgetPtr(widgetPtr)
{
	Q_ASSERT(widgetPtr != NULL);

	m_widgetPtr->setUpdatesEnabled(false);
}


inline CWidgetUpdateBlocker::~CWidgetUpdateBlocker()
{
	m_widgetPtr->setUpdatesEnabled(true);
}


} // namespace iwidgets




