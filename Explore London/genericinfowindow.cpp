/// The .cpp file for the bibliography window
///
/// Author: Seth Brown
/// Reviewed by: Chase Yates
/// Date: April 15, 2024

#include "genericinfowindow.h"
#include "ui_genericinfowindow.h"

#include <QDesktopServices>
#include <QFile>
#include <QString>
#include <QTextStream>

GenericInfoWindow::GenericInfoWindow(QWidget* parent)
    : QDialog(parent), ui(new Ui::GenericInfoWindow) {
    ui->setupUi(this);

    // when a link is clicked in the label, open it in a web browser
    connect(ui->contentLabel, &QLabel::linkActivated, this,
            &GenericInfoWindow::openLinkInBrowser);
}

GenericInfoWindow::~GenericInfoWindow() {
    delete ui;
}

void GenericInfoWindow::openWithContent(QString title, QString content) {
    setWindowTitle(title);
    ui->contentLabel->setText(content);
    open();
}

void GenericInfoWindow::openLinkInBrowser(const QString& url) const {
    QDesktopServices::openUrl(url);
}
