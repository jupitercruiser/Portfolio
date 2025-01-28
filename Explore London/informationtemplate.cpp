/// The .cpp file for information template window
///
/// Author: David Meredith
/// Reviewed by: Seth Brown and Melissa Umble
/// Date: April 15, 2024

#include "informationtemplate.h"
#include "QPixmap"
#include "location.h"
#include "ui_informationtemplate.h"

InformationTemplate::InformationTemplate(QWidget* parent)
    : QDialog(parent), ui(new Ui::InformationTemplate) {
    ui->setupUi(this);
}

InformationTemplate::~InformationTemplate() {
    delete ui;
}

void InformationTemplate::setUpInfo(Location location) {
    ui->titleLabel->setText(location.title);
    ui->themeLabel->setText(location.theme);
    ui->descriptionLabel->setText(location.information);

    QPixmap image(location.pictureFilepath);
    // scale the image to the size of the label
    int imageWidth = ui->imageLabel->width();
    int imageHeight = ui->imageLabel->height();
    image = image.scaled(imageWidth, imageHeight, Qt::KeepAspectRatio);
    ui->imageLabel->setPixmap(image);
}
