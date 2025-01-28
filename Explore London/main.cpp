/// The .cpp file for main that runs the app
///
/// Authors: Melissa Umble
/// Reviewed by: Seth Brown
/// Date: April 24, 2024

#include "mainwindow.h"
#include "model.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    Model model;
    MainWindow w(model);
    w.showMaximized();
    return a.exec();
}
