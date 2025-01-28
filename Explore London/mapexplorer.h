/// The .h file for a custom widget the implements a map
///
/// Author: Seth Brown, Chase Yates
/// Reviewed by: Chase Yates
/// Date: April 11, 2024

#ifndef MAPEXPLORER_H
#define MAPEXPLORER_H

#include "BadgeType.h"
#include "badgegraphicsitem.h"
#include "location.h"
#include "pingraphicsitem.h"

#include <QColor>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QImage>
#include <QMouseEvent>
#include <QPoint>
#include <QPointF>
#include <QResizeEvent>
#include <QWheelEvent>
#include <QWidget>

#include <functional>
#include <map>
#include <vector>

/// @brief A custom widget for the app's map
class MapExplorer : public QGraphicsView {
    Q_OBJECT

public:
    /// @brief Constructs the class
    /// @param parent This widget's parent
    MapExplorer(QWidget* parent = nullptr);

    /// @brief Destructs the class
    ~MapExplorer();

    /// @brief Adds a pin to the map
    /// @param x The x-coordinate where the bottom of the pin should be placed
    /// @param y The y-coordinate where the bottom of the pin should be placed
    /// @param icon The icon that should be displayed on the pin
    /// @param callback The callback that should be invoked when the pin is
    /// clicked
    void addPin(const Location& location, std::function<void()> callback);

protected:
    /// @brief Handles the mouse being pressed
    /// @param event The data associated with the event
    void mousePressEvent(QMouseEvent* event) override;

    /// @brief Handles a scroll event
    /// @param event The data associated with the event
    void wheelEvent(QWheelEvent* event) override;

    /// @brief Handles the widget being resized
    /// @param event The data associated with the event
    void resizeEvent(QResizeEvent* event) override;

public slots:
    /// @brief Zooms the canvas to the requested level
    /// @param newLevel The requested zoom level (from 0 to 100)
    void zoomLevelChangeRequested(int newLevel);

    /// @brief Adds a Box2D-driven badge to the scene
    /// @param type Which badge to add
    /// @param body The Box2D body associated with the badge
    /// @param icon The graphic to display for the badge
    void addPhysicsBadgeToScene(BadgeType type, b2Body* body, QIcon icon);

    /// @brief Removes a Box2D-driven badge from the scene
    /// @param type Which badge to remove
    void removePhysicsBadgeFromScene(BadgeType type);

    /// @brief Redraws the Box2D-driven badges
    void redrawBadges();

signals:
    /// @brief Emitted when the zoom level changes
    /// @param newLevel The new zoom level (from 0 to 100)
    void zoomLevelChanged(int newLevel);

private:
    /// @brief When the current zoom level changes, counterscales the pins so
    /// that they look like they stay the same size. Additionally, enforces a
    /// minimum scale so that when the user zooms in very far, the pin does grow
    /// (which helps it not look tiny compared to the surrounding map)
    void rescalePins();

    /// @brief The scene this widget is rendering
    QGraphicsScene* scene;

    /// @brief The map image
    QGraphicsPixmapItem* mapGraphicsItem;

    /// @brief The pins on the map
    std::vector<PinGraphicsItem*> pins;

    /// @brief How much the QGraphicsView needs to scale the scene in order to
    /// achieve a zoom level of 100%, where the full frame is displayed without
    /// having extra space around it
    qreal defaultScaleFactor;

    /// @brief The current zoom level, such that the scene's current scale
    /// factor is defaultScaleFactor * currentZoomLevel
    qreal currentZoomLevel;

    /// @brief Maps the value of the zoom slider to a zoom level
    /// @param zoomSliderVal The value of the zoom slider
    /// @return The zoom level
    /// @see zoomFracToSliderPercentage
    ///
    /// By default, a QSlider is linear across its range. However, in this
    /// case, this is not the behavior we want. Instead, this function takes a
    /// linear range from 0 to 100 and maps it an exponential range of zoom
    /// levels (where 100% means that the entire frame is visible but takes up
    /// all available space). Specifically, the value 0 maps to a zoom level of
    /// 90%, 50 maps to a zoom level of 200%, and 100 maps to a zoom level of
    /// 600%.
    static double sliderValToZoomLevel(int sliderVal);

    /// @brief Maps a zoom level to the value of the zoom slider
    /// @param zoom The zoom level
    /// @return The value of the zoom slider
    /// @see sliderValToZoomLevel for more information on how it works
    static int zoomLevelToSliderVal(double zoomLevel);

    /// @brief The path to the map image
    inline static const char* LONDON_MAP_PATH = ":/LondonMap.jpg";

    /// @brief When scaling the pins on the map, the minimum scale factor that
    /// should be used (this prevents the pins from looking tiny compared to the
    /// map)
    static constexpr qreal MIN_PIN_SCALE = 0.3;

    /// @brief The Box2D-driven badges in the scene
    std::map<BadgeType, BadgeGraphicsItem*> badges;
};

#endif // MAPEXPLORER_H
