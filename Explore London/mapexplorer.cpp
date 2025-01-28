/// The .cpp file for a custom widget the implements a map
///
/// Author: Seth Brown, Chase Yates
/// Reviewed by: Chase Yates
/// Date: April 11, 2024

#include "mapexplorer.h"
#include "badgegraphicsitem.h"
#include "location.h"
#include "pingraphicsitem.h"

#include <QApplication>
#include <QColor>
#include <QCoreApplication>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QImage>
#include <QMouseEvent>
#include <QPixmap>
#include <QPoint>
#include <QPointF>
#include <QRectF>
#include <QResizeEvent>
#include <QScrollBar>
#include <QWheelEvent>
#include <QWidget>
#include <cmath>
#include <functional>

MapExplorer::MapExplorer(QWidget* parent) : QGraphicsView(parent) {
    scene = new QGraphicsScene(this);
    setScene(scene);

    mapGraphicsItem = new QGraphicsPixmapItem(QPixmap(LONDON_MAP_PATH));
    scene->addItem(mapGraphicsItem);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragMode(ScrollHandDrag);
    setTransformationAnchor(AnchorUnderMouse);
}

MapExplorer::~MapExplorer() {
    for (auto& [_, badge] : badges) {
        delete badge;
    }
    delete scene;
}

void MapExplorer::addPin(const Location& location,
                         std::function<void()> callback) {
    double x = location.pinX;
    double y = location.pinY;

    PinGraphicsItem* newPin = new PinGraphicsItem(location, callback);

    // this math makes it so that the bottom of the pin ends up at the provided
    // x and y. if we didn't do this, the top-left of the pin would be placed
    // at the provided x and y
    newPin->setX(x - newPin->boundingRect().width() / 2.0);
    newPin->setY(y - newPin->boundingRect().height());

    scene->addItem(newPin);
    pins.push_back(newPin);
}

void MapExplorer::mousePressEvent(QMouseEvent* event) {
    if (event->buttons() == Qt::RightButton &&
        event->modifiers() == Qt::NoModifier) {
        QPointF location = mapToScene(event->pos());
        qInfo() << "the position you just right-clicked on is (" << location.x()
                << ", " << location.y() << ')';
    } else {
        QGraphicsView::mousePressEvent(event);
    }
}

void MapExplorer::wheelEvent(QWheelEvent* event) {
    // exponentially scale more as scrolling gets faster
    qreal scaleFactor = std::pow(2.0, event->angleDelta().y() / 180.0);
    scale(scaleFactor, scaleFactor);
    currentZoomLevel *= scaleFactor;
    rescalePins();
    emit zoomLevelChanged(zoomLevelToSliderVal(currentZoomLevel));
}

void MapExplorer::resizeEvent(QResizeEvent* event) {
    // set scaling factors
    qreal widthScalingFactor = static_cast<qreal>(event->size().width()) /
                               scene->itemsBoundingRect().width();
    qreal heightScalingFactor = static_cast<qreal>(event->size().height()) /
                                scene->itemsBoundingRect().height();
    defaultScaleFactor = std::fmin(widthScalingFactor, heightScalingFactor);
    currentZoomLevel = 1.0;

    rescalePins();
    fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
    emit zoomLevelChanged(zoomLevelToSliderVal(currentZoomLevel));
}

void MapExplorer::zoomLevelChangeRequested(int newLevel) {
    qreal newZoomLevel = sliderValToZoomLevel(newLevel);
    qreal scalingFactor = newZoomLevel / currentZoomLevel;
    scale(scalingFactor, scalingFactor);
    currentZoomLevel = newZoomLevel;
    rescalePins();
}

void MapExplorer::rescalePins() {
    // this makes it so that when the user zooms in and out, the pins appear to
    // remain the same size (which is done by scaling the pins oppositely to the
    // view). however, we enforce a minimum scale so that when the user zooms
    // in very far, the pin does grow (which helps it not look tiny compared to
    // the surrounding map)
    qreal newPinScale = std::fmax(1.0 / currentZoomLevel, MIN_PIN_SCALE);
    for (auto pin : pins) {
        pin->setScale(newPinScale);
    }
}

double MapExplorer::sliderValToZoomLevel(int sliderVal) {
    // this is an exponential function, where f(x) = a + b * e^c
    // I found these numbers by plugging in the results I wanted, specifically
    // that:
    // * f(0) = 0.9
    // * f(50) = 2.0
    // * f(100) = 6.0
    constexpr qreal a = 0.4827586206896553;
    constexpr qreal b = 0.4172413793103449;
    constexpr qreal c = 0.025819683626311316;

    return a + b * std::exp(c * static_cast<qreal>(sliderVal));
}

int MapExplorer::zoomLevelToSliderVal(double zoomLevel) {
    // see sliderValToZoomLevel to know where these numbers came from
    constexpr qreal a = 0.4827586206896553;
    constexpr qreal b = 0.4172413793103449;
    constexpr qreal c = 0.025819683626311316;

    // this just reverses the exponential function
    return static_cast<int>(std::round(std::log((zoomLevel - a) / b) / c));
}

void MapExplorer::addPhysicsBadgeToScene(BadgeType type, b2Body* body,
                                         QIcon icon) {
    BadgeGraphicsItem* item = new BadgeGraphicsItem(icon, body);
    item->icon = icon;
    item->physics = body;
    badges[type] = item;
    auto pos = body->GetPosition();
    item->setX(pos.x);
    item->setY(pos.y);
    item->setScale(10);
    scene->addItem(item);
}

void MapExplorer::removePhysicsBadgeFromScene(BadgeType type) {
    if (badges.count(type) <= 0) {
        return;
    }
    BadgeGraphicsItem* item = badges[type];
    scene->removeItem(item);
    badges.erase(type);
    delete item;
}

void MapExplorer::redrawBadges() {
    for (auto& [_, badge] : badges) {
        auto pos = badge->physics->GetPosition();
        badge->setX(pos.x);
        badge->setY(pos.y);
    }
}
