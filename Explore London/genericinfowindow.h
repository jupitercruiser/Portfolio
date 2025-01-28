/// The .h file for the bibliography window
///
/// Author: Seth Brown
/// Reviewed by: Chase Yates
/// Date: April 15, 2024

#ifndef GENERICINFOWINDOW_H
#define GENERICINFOWINDOW_H

#include <QDialog>

namespace Ui {
class GenericInfoWindow;
}

/// @brief A dialog that displays the bibliography
class GenericInfoWindow : public QDialog {
    Q_OBJECT

public:
    /// @brief Constructs the class
    /// @param parent This window's parent
    explicit GenericInfoWindow(QWidget* parent = nullptr);

    /// @brief Destructs the class
    ~GenericInfoWindow();

    /// @brief Opens the dialog with the specified title and content
    /// @param title The window's title
    /// @param content The content to display
    void openWithContent(QString title, QString content);

private slots:
    /// @brief Opens the given URL in the user's default web browser
    /// @param url The URL to open
    void openLinkInBrowser(const QString& url) const;

private:
    /// @brief The ui for this window, which was created in the Qt Form
    /// Designer
    Ui::GenericInfoWindow* ui;
};

#endif // GENERICINFOWINDOW_H
