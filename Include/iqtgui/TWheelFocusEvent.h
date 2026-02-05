#pragma once

#include <QWidget>
#include <QEvent>
#include <QAbstractSpinBox>
#include <QSlider>
#include <QDial>
#include <QAbstractItemView>
#include <QComboBox>


namespace iqtgui
{


// static functions

inline static void InstallWheelEventFilter(QObject* filterPtr, QWidget* parentPtr)
{
	if (!parentPtr) {
		return;
	}

	auto childrenList = parentPtr->findChildren<QWidget*>();
	for (auto it : childrenList)
	{
		if (auto viewPtr = dynamic_cast<QAbstractItemView*>(it))
		{
			viewPtr->viewport()->installEventFilter(filterPtr);
			continue;
		}

		if (dynamic_cast<QAbstractSpinBox*>(it) ||
			dynamic_cast<QSlider*>(it) ||
			dynamic_cast<QDial*>(it) ||
			dynamic_cast<QComboBox*>(it))
		{
			it->installEventFilter(filterPtr);
			it->setFocusPolicy(Qt::StrongFocus);
			continue;
		}
	}
}


inline static bool HandleWheelEvent(QObject* object, QEvent* event)
{
	// #10124: if the event is a wheel event and the editor does not have focus, ignore it
	// this is needed to prevent the wheel event from being passed to the parent widget
	// which may cause unwanted behavior (e.g. scrolling in a table view)
	// this is needed for all widgets that are not ACF classes (e.g. QSpinBox, QSlider, etc.)

	if (event->type() == QEvent::Wheel) {
		if (auto editorPtr = dynamic_cast<QWidget*>(object)) {
			if (!editorPtr->hasFocus()) {
				return true;
			}
		}
	}

	return false;
}


// use this class to install event filter for wheel events for non-ACF classes

template<class Widget = QWidget>
class TWheelFocusEventFilter : public Widget
{
public:
	TWheelFocusEventFilter(QWidget* parent = nullptr) : Widget(parent) 
	{}

protected:
	// reimplemented (QObject)
	virtual bool eventFilter(QObject* object, QEvent* event) override
	{
		// #10124
		if (HandleWheelEvent(object, event)) {
			return true;
		}
		else
			// pass the event to the parent class
			return Widget::eventFilter(object, event);
	}
};



// use this class to install event filter for wheel events for ACF classes

template<class GUIClass>
class TWheelFocusEvent : public GUIClass
{
public:
	typedef GUIClass BaseClass;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated() override
	{
		BaseClass::OnGuiCreated();
		
		// install event filter for wheel events
		InstallWheelEventFilter(this, GUIClass::GetQtWidget());
	}

protected:
	// reimplemented (QObject)
	virtual bool eventFilter(QObject* object, QEvent* event) override
	{
		// #10124
		if (HandleWheelEvent(object, event)) {
			return true;
		}
		else
			// pass the event to the parent class
			return BaseClass::eventFilter(object, event);
	}
};


} // ns
