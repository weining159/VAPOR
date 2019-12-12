#include <QEvent>
#include <QWidget>

#include "VContainer.h"

namespace {
    int LEFT_MARGIN   = 0;
    int TOP_MARGIN    = 0;
    int RIGHT_MARGIN  = 0;
    int BOTTOM_MARGIN = 0;
}

VContainer::VContainer() 
: QWidget() 
{
    QHBoxLayout* layout = new QHBoxLayout;
    layout->setContentsMargins(
        LEFT_MARGIN,
        TOP_MARGIN,
        RIGHT_MARGIN,
        BOTTOM_MARGIN
    );
    setLayout( layout );

    setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Preferred );
}

MouseWheelWidgetAdjustmentGuard::MouseWheelWidgetAdjustmentGuard(QObject *parent) : QObject(parent)
{
}

bool MouseWheelWidgetAdjustmentGuard::eventFilter(QObject *o, QEvent *e)
{
    const QWidget* widget = static_cast<QWidget*>(o);
    if (e->type() == QEvent::Wheel && widget && !widget->hasFocus())
    {
        e->ignore();
        return true;
    }

    return QObject::eventFilter(o, e);
}
