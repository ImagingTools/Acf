#ifndef iqtgui_CTreeWidgetFilter_included
#define iqtgui_CTreeWidgetFilter_included


#include <QtGui/QTreeWidget>

#include "iqtgui/iqtgui.h"


namespace iqtgui
{


/**
	Provide some corrections in \c QTreeWidget object.
	Objects of this class are automatically registered by slave objects and should not be deleted manually.
	Please create instance of this class on heap.
*/
class CTreeWidgetFilter: public QObject
{
public:
	typedef QObject BaseClass;

	/**
		Constructor.
		\param	slavePtr	pointer to filtered slave tree object.
							It cannot be NULL.
	*/
	CTreeWidgetFilter(QTreeWidget* slavePtr);

protected:
	// reimplemented (QObject)
	virtual bool eventFilter(QObject* sourcePtr, QEvent* eventPtr);

private:
	QTreeWidget& m_slave;
};


} // namespace iqtgui


#endif // !iqtgui_CTreeWidgetFilter_included


