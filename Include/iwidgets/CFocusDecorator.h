// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QtGlobal>
#include <QtCore/QMap>
#include <QtCore/QEvent>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QGraphicsEffect>
#include <QtWidgets/QWidget>
#else
#include <QtGui/QGraphicsEffect>
#include <QtGui/QWidget>
#endif

// ACF includes
#include <istd/IPolymorphic.h>


namespace iwidgets
{


/*
	Helper class for creating widget decorations based on graphics effects, such drop shadow.
*/
class CFocusDecorator: public QObject
{
	Q_OBJECT
public:
	class IGraphicsEffectFactory: virtual public istd::IPolymorphic
	{
	public:
		virtual std::unique_ptr<QGraphicsEffect> CreateEffect() const = 0;
	};

	explicit CFocusDecorator(QObject* parentPtr = NULL);
	virtual ~CFocusDecorator();

	bool RegisterWidget(QWidget* widgetPtr, IGraphicsEffectFactory* effectFactoryPtr);
	void UnregisterWidget(QWidget* widgetPtr);
	void UnregisterAllWidgets();

protected:
	// reimplemented (QObject)
	virtual bool eventFilter(QObject* objectPtr, QEvent* eventPtr) override;

private Q_SLOTS:
	void OnObjectDestroyed(QObject* objectPtr);

private:
	typedef QMap<QObject*, IGraphicsEffectFactory*> WidgetEffectsMap;
	WidgetEffectsMap m_widgetEffectsMap;
};


} // namespace iwidgets


