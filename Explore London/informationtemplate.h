/// The .h file for information template window
///
/// Author: David Meredith
/// Reviewed by: Seth Brown and Melissa Umble
/// Date: April 15, 2024

#ifndef INFORMATIONTEMPLATE_H
#define INFORMATIONTEMPLATE_H

#include "location.h"

#include <QDialog>

namespace Ui {
class InformationTemplate;
}

class InformationTemplate : public QDialog {
    Q_OBJECT

public:
    /// @brief the constructor for the information template window
    /// @param parent a pointer to the QWidget parent
    explicit InformationTemplate(QWidget* parent = nullptr);

    /// @brief the destructor for an information template
    ~InformationTemplate();

    /// @brief sets up the information for the given location onto the ui
    /// @param location the location to get information from to populate the
    /// window
    void setUpInfo(Location location);

private:
    Ui::InformationTemplate* ui;
};

#endif // INFORMATIONTEMPLATE_H
