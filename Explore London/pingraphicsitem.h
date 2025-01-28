/// The .h file for a graphics item that is a pin on the map
///
/// Author: Seth Brown
/// Reviewed by: David Meredith
/// Date: April 11, 2024

#ifndef PINGRAPHICSITEM_H
#define PINGRAPHICSITEM_H

#include "location.h"

#include <QColor>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QImage>
#include <QPoint>
#include <QSize>
#include <QString>
#include <functional>

/// @brief A pin on the map
class PinGraphicsItem : public QGraphicsPixmapItem {
public:
    /// @brief Constructs the class
    /// @param location The location's information
    /// @param callback The callback function that should be invoked when the
    /// pin is clicked
    PinGraphicsItem(const Location& location, std::function<void()> callback);

protected:
    /// @brief Handles the mouse being pressed
    /// @param event The data associated with the event
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    /// @brief Puts the icon image on top of the pin graphic
    /// @param pin The pin graphic
    /// @param icon The icon image
    /// @returns The pin graphic with the icon overlaid on it
    static QImage overlayIconOnPin(QImage pin, QImage icon);

    /// @brief The callback that will be invoked when the pin is clicked
    std::function<void()> callback;

    /// @brief A mapping from location each category to the path of its pin
    /// graphic
    inline static const QMap<QString, QString> PIN_PATHS{
        {"Government", ":/pins/government.png"},
        {"Religion", ":/pins/religion.png"},
        {"Royal Family", ":/pins/royal_family.png"},
        {"Crimes", ":/pins/crimes.png"},
        {"Arts and Culture", ":/pins/arts_and_culture.png"},
        {"Bridges", ":/pins/bridges.png"},
    };

    /// @brief The path to the default pin graphic (used if the location
    /// category is not recognized)
    inline static const QString DEFAULT_PIN_PATH = ":/pins/default.png";

    /// @brief The required size of the icon image passed by the user of this
    /// class
    static constexpr QSize ICON_SIZE = QSize(320, 320);

    /// @brief Where the top-left corner of the icon image should be placed in
    /// the pin graphic
    static constexpr QPoint ICON_OFFSET = QPoint(40, 40);
};

#endif // PINGRAPHICSITEM_H
