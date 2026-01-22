// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QEvent>


namespace iqt
{


class CDesignThemeEvent: public QEvent
{
public:
	typedef QEvent BaseClass;

	static int s_eventType;

	CDesignThemeEvent(const QByteArray& themeId);

	QByteArray GetThemeId() const;

#if QT_VERSION >= 0x600000
	// reimplemented (QEvent)
	virtual QEvent* clone() const override;
#endif
private:
	QByteArray m_themeId;
};


} // namespace iqt


