/// The .h file for Name Button which extends QPushButton
///
/// Author: Melissa Umble
/// Reviewed by: Chase Yates
/// Date: 4/24/2024

#ifndef NAMEBUTTON_H
#define NAMEBUTTON_H
#include <QPushButton>
#include <QString>

/// @brief Name button class that extends QPushButton
class nameButton : public QPushButton {
    Q_OBJECT

public:
    /// @brief Constructs a name button
    /// @param parent of button
    nameButton(QWidget* parent = nullptr);

signals:
    /// @brief Sends the name of a clicked button
    /// @param name of button
    void getButtonNameOnClick(QString name);

protected:
    /// @brief Overrides mouse press event
    void mousePressEvent(QMouseEvent* event) override;
};

#endif // NAMEBUTTON_H
