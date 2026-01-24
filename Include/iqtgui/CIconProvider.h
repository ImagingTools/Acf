// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <iqtgui/iqtgui.h>


namespace iqtgui
{


class CIconProvider
{
public:
	virtual QIcon GetIcon(const QString& iconName) const;
	virtual QString GetIconPath(const QString& iconName) const;
	virtual QString GetStyleSheetPath(const QString& styleSheetPath) const;

	QByteArray GetCurrentThemeId() const;
	void SetThemeId(const QByteArray& themeId);

protected:
	virtual QIcon CreateIcon(const QString& iconName) const;

private:
	QByteArray m_themeId;
};


}


