/*
 * This is the source file for the physics badge graphics item that shows up on
 * the map. Contains icon data and physics data
 * Author: Chase Yates
 * Reviewed by: Seth Brown
 * Date: 4/24/2024
 */

#include "badgegraphicsitem.h"

BadgeGraphicsItem::BadgeGraphicsItem(QIcon _icon, b2Body* _physics)
    : icon(_icon), physics(_physics) {
    setPixmap(icon.pixmap(QSize(64, 64)));
}
