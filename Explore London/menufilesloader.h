/// The .h file for the menufilesloader
///
/// Author: Seth Brown
/// Reviewed by: David Meredith
/// Date: April 24, 2024

#ifndef MENUFILESLOADER_H
#define MENUFILESLOADER_H

#include <QString>

/// @brief Loads the "Gameplay Instructions" from its file
/// @returns The contents of the file
QString loadInstructions();

/// @brief Loads and preprocesses the "Bibliography" from its file
/// @returns The HTML produced by the processing step
QString loadBibliography();

/// @brief Loads the "Credits" from its file
/// @returns The contents of the file
QString loadCredits();

#endif // MENUFILESLOADER_H
