/*
 * This is the header file for the physics badge graphics item that shows up on
 * the map. Contains icon data and physics data
 * Author: Chase Yates
 * Reviewed by: Seth Brown, David Meredith
 * Date: 4/24/2024
 */

#ifndef BADGEGRAPHICSITEM_H
#define BADGEGRAPHICSITEM_H

#include <Box2D/Box2D.h>
#include <QGraphicsPixmapItem>
#include <QIcon>

class BadgeGraphicsItem : public QGraphicsPixmapItem {
public:
    /// @brief the constructor for a badge graphic
    /// @param icon the icon of the badge
    /// @param physics the box2d physics
    BadgeGraphicsItem(QIcon _icon, b2Body* _physics);
    QIcon icon;
    b2Body* physics;
};

#endif // BADGEGRAPHICSITEM_H
