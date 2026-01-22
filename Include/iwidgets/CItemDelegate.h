// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QItemDelegate>
#else
#include <QtGui/QItemDelegate>
#endif


// ACF includes
#include <iwidgets/iwidgets.h>


namespace iwidgets
{


/**
	Standard ACF item view delegator.
*/
class CItemDelegate: public QItemDelegate
{
public:
	typedef QItemDelegate BaseClass;

	CItemDelegate(int itemHeight = 20, QObject* parent = NULL);

	/**
		Set the item height.
	*/
	void SetItemHeight(int itemHeight);

	/**
		Get the item height.
	*/
	int GetItemHeight() const;

	// reimplemented (QItemDelegate)
	virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
	int m_itemHeight;
};


} // namespace iwidgets




