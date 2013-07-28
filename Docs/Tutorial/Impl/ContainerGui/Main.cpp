// ACF includes
#include "icomp/TSimComponentWrap.h"
#include "icomp/TSimComponentsFactory.h"

// ACF package includes
#include "BasePck/BasePck.h"
#include "QtPck/QtPck.h"
#include "QtGuiPck/QtGuiPck.h"


int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(iqtgui);

	QApplication app(argc, argv);

	iqtgui::CMultiPageWidget emptyContainer;
	emptyContainer.setWindowTitle("Empty container");
	emptyContainer.showMaximized();

	iqtgui::CMultiPageWidget tabContainer(NULL, iqtgui::CMultiPageWidget::DT_TAB_WIDGET);
	tabContainer.setWindowTitle("Tab container");

	QLabel page1("Page 1");
	tabContainer.InsertPage(&page1, "Page 1");
	tabContainer.SetPageIcon(0, QIcon(":/Icons/StateOk.svg"));

	QLabel page2("Page 2");
	tabContainer.InsertPage(&page2, "Page 2");
	tabContainer.SetPageIcon(1, QIcon(":/Icons/StateWarning.svg"));

	QLabel page3("Page 3");
	tabContainer.InsertPage(&page3, "Page 3");
	tabContainer.SetPageIcon(2, QIcon(":/Icons/StateInvalid.svg"));

	tabContainer.showMaximized();

	return app.exec();
}

