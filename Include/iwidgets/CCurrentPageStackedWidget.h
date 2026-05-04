#pragma once

#include <QtWidgets/QStackedWidget>

namespace iwidgets
{

class CCurrentPageStackedWidget : public QStackedWidget
{
	Q_OBJECT

public:
	explicit CCurrentPageStackedWidget(QWidget* parentPtr = nullptr);

	QSize sizeHint() const override;
	QSize minimumSizeHint() const override;
};

} // namespace iwidgets