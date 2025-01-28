/// The .cpp file for Location, which will store all the needed information of a
/// location to display
///
/// Author: David Meredith
/// Reviewed by: Brady Spinti
/// Date: April 24, 2024

#include "location.h"
#include <QString>

Location::Location() {}

Location::Location(QString title, QString theme, QString information,
                   QString pictureFilepath, QString iconFilepath, qreal pinX,
                   qreal pinY) {
    this->title = title;
    this->theme = theme;
    this->information = information;
    this->pictureFilepath = pictureFilepath;
    this->iconFilepath = iconFilepath;
    this->pinX = pinX;
    this->pinY = pinY;
}
