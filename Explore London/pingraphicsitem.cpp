/// The .cpp file for a graphics item that is a pin on the map
///
/// Author: Seth Brown
/// Reviewed by: David Meredith
/// Date: April 11, 2024

#include "pingraphicsitem.h"
#include "location.h"

#include <QColor>
#include <QCursor>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QSize>
#include <QString>
#include <QtCore/qlogging.h>
#include <functional>

PinGraphicsItem::PinGraphicsItem(const Location& location,
                                 std::function<void()> callback)
    : QGraphicsPixmapItem(), callback{callback} {
    QImage icon(location.iconFilepath);
    if (icon.size() != ICON_SIZE) {
        qFatal("The icon needs to be %dx%d", ICON_SIZE.width(),
               ICON_SIZE.height());
    }

    QString pinPath = DEFAULT_PIN_PATH;
    auto pathIter = PIN_PATHS.find(location.theme);
    if (pathIter != PIN_PATHS.end()) {
        pinPath = *pathIter;
    }

    QImage toDisplay = overlayIconOnPin(QImage(pinPath), icon);
    setPixmap(QPixmap::fromImage(toDisplay));

    // this makes it so that when the pin is scaled, it happens around the
    // bottom of the pin
    setTransformOriginPoint(boundingRect().width() / 2.0,
                            boundingRect().height());

    // indicates the pin is clickable when hovered over
    setCursor(Qt::PointingHandCursor);
}

void PinGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->buttons() == Qt::LeftButton &&
        event->modifiers() == Qt::NoModifier) {
        callback();
    }
}

QImage PinGraphicsItem::overlayIconOnPin(QImage pin, QImage icon) {
    QImage result(pin);
    QPainter painter(&result);
    painter.drawImage(ICON_OFFSET, icon);
    painter.end();
    return result;
}
