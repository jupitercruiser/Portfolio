/// The .h file for Location, which will store all the needed information of a
/// location to display
///
/// Author: David Meredith
/// Reviewed by: Brady Spinti
/// Date: April 24, 2024

#ifndef LOCATION_H
#define LOCATION_H

#include <QString>

class Location {

public:
    /// @brief the default constructor for a location
    Location();

    /// @brief the constructor for a location that requires all fields that a
    /// Location has
    /// @param title the title of the location
    /// @param theme the theme the location is a part of
    /// @param information the text information that goes in the popup window
    /// @param pictureFilepath the filepath of the picture that goes in the
    /// popup window
    /// @param iconFilepath the filepath of the icon picture to put on the map
    /// @param pinX the x coordinate on the map the location is at
    /// @param pinY the y coordinate on the map the location is at
    Location(QString title, QString theme, QString information,
             QString pictureFilepath, QString iconFilepath, qreal pinX,
             qreal pinY);

    // fields
    QString title;
    QString theme;
    QString information;
    QString pictureFilepath;
    QString iconFilepath;
    qreal pinX;
    qreal pinY;
};

#endif // LOCATION_H
