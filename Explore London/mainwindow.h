/// The .h file for the MainWindow of the Explore London educational app
///
/// Authors: Mel Umble, Brandy Cervantes, David Meredith, Seth Brown, Brady Spinti
/// Reviewed by: Brady Spinti
/// Date: April 24, 2024

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "celebrationscreen.h"
#include "genericinfowindow.h"
#include "informationtemplate.h"
#include "model.h"
#include "quiz.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/// @brief Main Window for Explore London App
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /// @brief Constructs MainWindow
    /// @param model app's model containing logic
    /// @param parent parent class
    MainWindow(Model& model, QWidget* parent = nullptr);

    /// @brief Main Window destructor
    ~MainWindow();

public slots:

    /// @brief Opens the location screen ui and pops up the location screen in a
    /// seperate window
    /// @param location - the information to populate the location screen to
    /// have the actual data for the location aka the text and images for the
    /// specific pin clicked
    void openLocationPopUp(const Location& location);

    /// @brief Opens the quiz screen ui and pops up the quiz screen in a
    /// seperate window
    /// @param quizContent - the information to populate the quiz screen to have
    /// the actual data for the quiz aka the questions and answers for which
    /// ever quiz was clicked on
    void openQuizPopUp(const QuestionsAnswers& quizContent);

    /// @brief Opens the instruction screen in a seperate window
    void openInstructions();

    /// @brief Opens the bibliography screen in a seperate window
    void openBibliography();

    /// @brief Opens the credits screen in a seperate window
    void openCredits();

    /// @brief closes the quiz window after you finished the quiz
    void closeQuiz();

    /// @brief the badge appears in its spot on the window and allows the user
    /// to now click it to make it appear and move around the screen
    /// @param type - which badge button and image to activate
    void enableBadgeIcon(BadgeType type);

    /// @brief makes it so that you can't retake teh quizzes and can no longer
    /// click ontheir buttons after you passed them
    /// @param quizName - which quiz button to disable
    void disableQuizButtonAfterPass(QString quizName);

    /// @brief Opens the celebration screen ui and pops up the celebrtion screen
    /// in a seperate window
    void openCelebrationScreen();

private:
    void setupPins();
    Ui::MainWindow* ui;
    InformationTemplate examplePopUp;
    Quiz quizPopUp;
    GenericInfoWindow infoWindowPopup;

    /// @brief The gameplay instructions to show in the dialog
    QString instructionsContent;

    /// @brief The parsed and HTML-rendered bibliography to show in the dialog
    QString bibliographyContent;

    /// @brief The credits to show in the dialog
    QString creditsContent;

    CelebrationScreen celebrationPopUp;
};
#endif // MAINWINDOW_H
