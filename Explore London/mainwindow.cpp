/// The .cpp file that contains the implementation for MainWindow
///
/// Authors: Mel Umble, Brandy Cervantes, David Meredith, Seth Brown, Brady Spinti
/// Reviewed by: Brady Spinti
/// Date: April 24, 2024

#include "mainwindow.h"
#include "menufilesloader.h"
#include "model.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Model& model, QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      instructionsContent(loadInstructions()),
      bibliographyContent(loadBibliography()), creditsContent(loadCredits()) {
    ui->setupUi(this);

    // set badges as empty
    QIcon badgeSpot(":/images/BadgeSpot.png");
    ui->pushButton1_bridges->setIcon(badgeSpot);
    ui->pushButton2_art_culture->setIcon(badgeSpot);
    ui->pushButton3_crimes->setIcon(badgeSpot);
    ui->pushButton4_government->setIcon(badgeSpot);
    ui->pushButton5_religion->setIcon(badgeSpot);
    ui->pushButton6_royal_family->setIcon(badgeSpot);

    // connect badge buttons
    connect(ui->pushButton1_bridges, &QPushButton::clicked, &model,
            &Model::toggleBridgeBadgeRequested);
    connect(ui->pushButton2_art_culture, &QPushButton::clicked, &model,
            &Model::toggleArtCultureBadgeRequested);
    connect(ui->pushButton3_crimes, &QPushButton::clicked, &model,
            &Model::toggleCrimesBadgeRequested);
    connect(ui->pushButton4_government, &QPushButton::clicked, &model,
            &Model::toggleGovernmentBadgeRequested);
    connect(ui->pushButton5_religion, &QPushButton::clicked, &model,
            &Model::toggleReligionBadgeRequested);
    connect(ui->pushButton6_royal_family, &QPushButton::clicked, &model,
            &Model::toggleRoyalFamilyBadgeRequested);

    // connection for enabling corresponding badge icon when badges are unlocked
    connect(&model, &Model::enableBadgeIcon, this,
            &MainWindow::enableBadgeIcon);

    // connect quiz result to unlock badge instead of just calling a function so
    // others can use quizResults if need be
    connect(&model, &Model::quizResults, &model, &Model::unlockBadge);

    // connect for physics update to update map explorer view
    connect(&model, &Model::addBadgeToView, ui->mapExplorer,
            &MapExplorer::addPhysicsBadgeToScene);
    connect(&model, &Model::removeBadgeFromView, ui->mapExplorer,
            &MapExplorer::removePhysicsBadgeFromScene);

    // connect graphics redraw for map
    connect(&model, &Model::redrawMap, ui->mapExplorer,
            &MapExplorer::redrawBadges);

    connect(&model, &Model::updateViewPopup, this,
            &MainWindow::openLocationPopUp);

    // menu bar
    connect(ui->actionHow_to_Play, &QAction::triggered, this,
            &MainWindow::openInstructions);
    connect(ui->actionSources, &QAction::triggered, this,
            &MainWindow::openBibliography);
    connect(ui->actionCredits, &QAction::triggered, this,
            &MainWindow::openCredits);

    connect(ui->zoomSlider, &QSlider::valueChanged, ui->mapExplorer,
            &MapExplorer::zoomLevelChangeRequested);
    connect(ui->mapExplorer, &MapExplorer::zoomLevelChanged, ui->zoomSlider,
            &QSlider::setValue);

    // quiz connections
    connect(ui->royalFamQuizButton, &nameButton::getButtonNameOnClick, &model,
            &Model::getQuizInfo);
    connect(ui->crimesQuizButton, &nameButton::getButtonNameOnClick, &model,
            &Model::getQuizInfo);
    connect(ui->governmentQuizButton, &nameButton::getButtonNameOnClick, &model,
            &Model::getQuizInfo);
    connect(ui->religionQuizButton, &nameButton::getButtonNameOnClick, &model,
            &Model::getQuizInfo);
    connect(ui->bridgesQuizButton, &nameButton::getButtonNameOnClick, &model,
            &Model::getQuizInfo);
    connect(ui->artsCultureQuizButton, &nameButton::getButtonNameOnClick,
            &model, &Model::getQuizInfo);

    connect(&model, &Model::UpdateViewQuizPopup, this,
            &MainWindow::openQuizPopUp);

    connect(&quizPopUp, &Quiz::closeWindow, this, &MainWindow::closeQuiz);

    connect(&model, &Model::disableQuizButton, this,
            &MainWindow::disableQuizButtonAfterPass);

    // quiz to model connections
    connect(&quizPopUp, &Quiz::answersCollected, &model,
            &Model::checkQuizAnswers);

    connect(&model, &Model::quizResults, &quizPopUp,
            &Quiz::quizGradedUpdateWindow);

    // add pins
    connect(&model, &Model::allBadgesUnlocked, this,
            &MainWindow::openCelebrationScreen);

    model.addPinsToMap(ui->mapExplorer);

    // initialize physics
    model.initBox2D();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::openLocationPopUp(const Location& location) {
    examplePopUp.setUpInfo(location);
    examplePopUp.setWindowTitle(location.title);
    examplePopUp.show();
}

void MainWindow::openQuizPopUp(const QuestionsAnswers& quizContent) {
    quizPopUp.setUpInfo(quizContent);
    quizPopUp.setWindowTitle(quizContent.theme + "Quiz");
    quizPopUp.show();
}

void MainWindow::openInstructions() {
    infoWindowPopup.openWithContent("Gameplay Instructions",
                                    instructionsContent);
}

void MainWindow::openBibliography() {
    infoWindowPopup.openWithContent("Bibliography", bibliographyContent);
}

void MainWindow::openCredits() {
    infoWindowPopup.openWithContent("Credits", creditsContent);
}

void MainWindow::closeQuiz() {
    quizPopUp.hide();
}

void MainWindow::enableBadgeIcon(BadgeType type) {
    switch (type) {
    case BadgeType::Bridges:
        ui->pushButton1_bridges->setIcon(QIcon(":/icons/bridges.png"));
        break;
    case BadgeType::ArtAndCulture:
        ui->pushButton2_art_culture->setIcon(QIcon(":/icons/art_culture.png"));
        break;
    case BadgeType::Crimes:
        ui->pushButton3_crimes->setIcon(QIcon(":/icons/crimes.png"));
        break;
    case BadgeType::Government:
        ui->pushButton4_government->setIcon(QIcon(":/icons/government.png"));
        break;
    case BadgeType::Religion:
        ui->pushButton5_religion->setIcon(QIcon(":/icons/religion.png"));
        break;
    case BadgeType::RoyalFamily:
        ui->pushButton6_royal_family->setIcon(
            QIcon(":/icons/royal_family.png"));
        break;
    }
}

void MainWindow::disableQuizButtonAfterPass(QString quizName) {
    if (quizName == "bridges") {
        ui->bridgesQuizButton->setEnabled(false);
    } else if (quizName == "royal family") {
        ui->royalFamQuizButton->setEnabled(false);
    } else if (quizName == "government") {
        ui->governmentQuizButton->setEnabled(false);
    } else if (quizName == "art and culture") {
        ui->artsCultureQuizButton->setEnabled(false);
    } else if (quizName == "crimes") {
        ui->crimesQuizButton->setEnabled(false);
    } else {
        ui->religionQuizButton->setEnabled(false);
    }
}

void MainWindow::openCelebrationScreen() {
    celebrationPopUp.show();
}
