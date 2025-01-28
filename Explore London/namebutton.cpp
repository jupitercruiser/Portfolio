/// The .cpp file that contains the implementation for a name button which
/// extends QPushButton
///
/// Author: Melissa Umble
/// Reviewed by: Chase Yates
/// Date: 4/24/2024

#include "namebutton.h"
#include <QPushButton>
#include <QString>

nameButton::nameButton(QWidget* parent) : QPushButton(parent) {}

void nameButton::mousePressEvent(QMouseEvent*) {
    emit getButtonNameOnClick(this->objectName());
}
