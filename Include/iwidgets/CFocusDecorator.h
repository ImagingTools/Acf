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


namespace iwidgets
{


/*
	Helper class for creating widget decorations based on graphics effects, such drop shadow.
*/
class CFocusDecorator: public QObject
{
public:
	CFocusDecorator(QObject* parentPtr = NULL);
	virtual ~CFocusDecorator();

	bool RegisterWidget(QWidget* widgetPtr, QGraphicsEffect* effectPtr);
	void UnregisterWidget(QWidget* widgetPtr);
	void UnregisterAllWidgets();

protected:
	// reimplemented (QObject)
	virtual bool eventFilter(QObject* objectPtr, QEvent* eventPtr);

private:
	typedef QMap<QWidget*, QGraphicsEffect*> WidgetEffectsMap;
	WidgetEffectsMap m_widgetEffectsMap;
};


} // namespace iwidgets


