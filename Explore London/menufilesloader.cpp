/// The .cpp file for the menufilesloader
///
/// Author: Seth Brown
/// Reviewed by: David Meredith
/// Date: April 24, 2024

#include "menufilesloader.h"

#include <QFile>
#include <QString>
#include <QTextStream>

/// @brief Reads a file's contents into a QString
/// @param path The path of the file to read
/// @returns The contents of the file
QString readFileContents(QString path) {
    QFile file(path);
    file.open(QIODeviceBase::ReadOnly);
    return QString(file.readAll());
}

/// @brief The path to the file where the gameplay instructions are stored
const char* INSTRUCTIONS_FILE = ":/instructions.html";

/// @brief Loads the "Gameplay Instructions" from its file
/// @returns The contents of the file
QString loadInstructions() {
    return readFileContents(INSTRUCTIONS_FILE);
}

/// @brief The path to the file where the credits are stored
const char* CREDITS_FILE = ":/credits.html";

/// @brief Loads the "Credits" from its file
/// @returns The contents of the file
QString loadCredits() {
    return readFileContents(CREDITS_FILE);
}

/// @brief The current state of the bibliography file parser
///
/// parseBibFile uses a state machine to parse the bibliography file. These
/// represent the possible states of the parser
enum class BibliographyReadState {
    /// @brief The next line is expected to be a category of sites
    AwaitingCategory,

    /// @brief The next line is expected to be the name of a site
    AwaitingSite,

    /// @brief The next line is expected to be the URL of a source used in
    /// researching a specific site
    AwaitingUrl,
};

/// @brief Parses a bibliography file (which is our custom format) and returns
/// an HTML-formatted bibliography
QString parseBibFile(QString filename) {
    // supposedly using the stream is more efficient with allocations?
    QString renderedBib = "<h1>Bibliography</h1>";
    QTextStream bibWriter(&renderedBib);

    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QTextStream fileReader(&file);

    QString line;
    BibliographyReadState state = BibliographyReadState::AwaitingCategory;
    while (!fileReader.atEnd()) {
        line = fileReader.readLine();
        switch (state) {
        case BibliographyReadState::AwaitingCategory:
            // if we have many empty lines, make things easier by ignoring
            // them. as long as two sections are separated by at least two empty
            // lines, they will be considered as two separate categories
            if (!line.isEmpty()) {
                bibWriter << "<h2>" << line << "</h2>";
                state = BibliographyReadState::AwaitingSite;
            }
            break;
        case BibliographyReadState::AwaitingSite:
            if (!line.isEmpty()) {
                bibWriter << "<h3>" << line << "</h3>";
                state = BibliographyReadState::AwaitingUrl;
            } else {
                state = BibliographyReadState::AwaitingCategory;
            }
            break;
        case BibliographyReadState::AwaitingUrl:
            if (!line.isEmpty()) {
                bibWriter << "<p>" << line << "</p>";
            } else {
                state = BibliographyReadState::AwaitingSite;
            }
            break;
        }
    }

    file.close();

    return renderedBib;
}

/// @brief The path to the file where the bibliography is stored
const char* BIBLIOGRAPHY_FILE = ":/bibliography.txt";

/// @brief Loads and preprocesses the "Bibliography" from its file
/// @returns The HTML produced by the processing step
QString loadBibliography() {
    return parseBibFile(BIBLIOGRAPHY_FILE);
}
