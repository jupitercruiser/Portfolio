/// The .h file for a Quiz ui window that displays
/// a quiz and grades it based on user input
///
/// Authors: Brandy Cervantes and Melissa Umble
/// Reviewed by: Brady Spinti
/// Date: April 24, 2024

#ifndef QUIZ_H
#define QUIZ_H

#include "questionsanswers.h"
#include <QDialog>
#include <QTimer>
#include <QVector>

namespace Ui {
class Quiz;
}

/// @brief Quiz for Explore London App
class Quiz : public QDialog {
    Q_OBJECT

public:
    /// @brief Quiz Constructs the Quiz ui window
    explicit Quiz(QWidget* parent = nullptr);

    /// @brief Destructs a Quiz ui window
    ~Quiz();

    bool passed;
    QVector<QString> userAnswers;

private:
    Ui::Quiz* ui;

    /// @brief resetSelections This method removes clears the user's previous
    /// answers to the quiz
    void resetSelections();

    /// @brief Hides failed label after time has passed
    void hideFailedLabel();

    /// @brief Hides passed label after time has passed
    void hidePassedLabel();

signals:
    /// @brief answersCollected This signal is emitted when the user answers
    /// have been collected
    /// @param userAnswers Vector of strings containing the answers the user
    /// selected
    void answersCollected(QVector<QString> userAnswers);

    /// @brief closeWindow This signal is emitted when the user clicks the
    /// cancel button
    void closeWindow();

public slots:
    /// @brief setUpInfo This slot puts the quiz questions and options onto the
    /// Quiz ui by extracting them from the QuestionsAnswers object
    /// @param qAnda QuestionsAnswers object containing all of the information
    /// for a quiz
    void setUpInfo(QuestionsAnswers qAnda);

    /// @brief sendUserAnswers This slot extracts the user's answers from the
    /// Quiz ui and loads them into
    ///  a userAnswers array. It then emits a signal to the model so that the
    ///  answers can be graded.
    void sendUserAnswers();

    /// @brief submitCancelClicked This slot emits a signal to the MainWindow so
    /// that the Quiz ui can be closed.
    void submitCancelClicked();

    /// @brief quizGradedUpdateWindow
    /// @param passed if quiz has been passed
    void quizGradedUpdateWindow(bool passed);
};

#endif // QUIZ_H
