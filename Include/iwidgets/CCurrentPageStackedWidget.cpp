#include <iwidgets/CCurrentPageStackedWidget.h>

namespace iwidgets
{

CCurrentPageStackedWidget::CCurrentPageStackedWidget(QWidget* parentPtr)
    : QStackedWidget(parentPtr)
{
    connect(this, &QStackedWidget::currentChanged, this, [this](int) {
        updateGeometry();
        adjustSize();

        if (QWidget* parentPtr = parentWidget()) {
            parentPtr->updateGeometry();
            parentPtr->adjustSize();
        }
    });
}

QSize CCurrentPageStackedWidget::sizeHint() const
{
    if (QWidget* currentPtr = currentWidget()) {
        return currentPtr->sizeHint();
    }

    return QStackedWidget::sizeHint();
}

QSize CCurrentPageStackedWidget::minimumSizeHint() const
{
    if (QWidget* currentPtr = currentWidget()) {
        return currentPtr->minimumSizeHint();
    }

    return QStackedWidget::minimumSizeHint();
}

} // namespace iwidgets