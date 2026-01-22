// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iwidgets/iwidgets.h>


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
	#include <QtWidgets/QWidget>
	#include <QtWidgets/QLayoutItem>
	#include <QtWidgets/QLayout>
#else
	#include <QtGui/QWidget>
	#include <QtGui/QLayoutItem>
	#include <QtGui/QLayout>
#endif


namespace iwidgets
{


void ClearLayout(QLayout* layout)
{
	if (layout != NULL){
		while (QLayoutItem* item = layout->takeAt(0)){
			QWidget* widgetPtr = item->widget();
			if (widgetPtr != NULL){
				delete widgetPtr;
			}
		
			QLayout* childLayoutPtr = item->layout();
			if (childLayoutPtr != NULL){
				ClearLayout(childLayoutPtr);
			}

			delete item;
		}
	}
}


} // namespace iwidgets


