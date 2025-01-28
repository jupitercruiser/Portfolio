/// The .cpp file for a Quiz ui window that implements
/// all of its necessary components and reads the user's
/// answers
///
/// Author: Brandy Cervantes and Melissa Umble
/// Reviewed by: Brady Spinti
/// Date: April 24, 2024

#include "quiz.h"
#include "questionsanswers.h"
#include "ui_quiz.h"
#include <QDebug>
#include <QTimer>
#include <QVector>
#include <algorithm>
#include <chrono>
#include <random>

using std::shuffle;

Quiz::Quiz(QWidget* parent) : QDialog(parent), ui(new Ui::Quiz) {
    passed = false;
    ui->setupUi(this);

    // submit and cancel connections
    connect(ui->submitButton, &QAbstractButton::clicked, this,
            &Quiz::sendUserAnswers);

    connect(ui->cancelButton, &QAbstractButton::clicked, this,
            &Quiz::submitCancelClicked);

    // quiz passed/failed label setup
    ui->label->setStyleSheet(
        "border-style: outset; border-width: 4px; min-width: 10em; "
        "border-color: black; background-color: rgb(237, 109, 111); padding: "
        "6px;");
    ui->label->setVisible(false);
}

Quiz::~Quiz() {
    delete ui;
}

void Quiz::setUpInfo(QuestionsAnswers quizInfo) {
    // Write theme to ui
    ui->themeLabel->setText(quizInfo.theme + " Quiz");

    // Write questions to ui
    ui->multChoiceLabel->setText(quizInfo.multQuestion);
    ui->selectAllLabel->setText(quizInfo.selectAllQuestion);
    ui->trueFalseLabel->setText(quizInfo.trueFalseQuestion);

    // Vector of multiple choice options to randomize
    QVector<QString> multOptions{quizInfo.multAnswerA, quizInfo.multAnswerB,
                                 quizInfo.multAnswerC, quizInfo.multAnswerD};
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // randomize using shuffle
    shuffle(multOptions.begin(), multOptions.end(),
            std::default_random_engine(seed));

    // Write in options for multiple choice question
    ui->firstMultButton->setText(multOptions[0]);
    ui->secondMultButton->setText(multOptions[1]);
    ui->thirdMultButton->setText(multOptions[2]);
    ui->fourthMultButton->setText(multOptions[3]);

    // Vector of select all options to randomize
    QVector<QString> selectOptions{
        quizInfo.selectAllAnswerA, quizInfo.selectAllAnswerB,
        quizInfo.selectAllAnswerC, quizInfo.selectAllAnswerD};
    unsigned seed1 =
        std::chrono::system_clock::now().time_since_epoch().count();
    // randomize using shuffle
    shuffle(selectOptions.begin(), selectOptions.end(),
            std::default_random_engine(seed1));

    // Write in options for select all question
    ui->firstSelectBox->setText(selectOptions[0]);
    ui->secondSelectBox->setText(selectOptions[1]);
    ui->thirdSelectBox->setText(selectOptions[2]);
    ui->fourthSelectBox->setText(selectOptions[3]);

    // Write in options for true false question
    ui->trueButton->setText("True");
    ui->falseButton->setText("False");
}

void Quiz::sendUserAnswers() {
    if ((ui->firstMultButton->isChecked() ||
         ui->secondMultButton->isChecked() ||
         ui->thirdMultButton->isChecked() ||
         ui->fourthMultButton->isChecked()) &&
        (ui->firstSelectBox->isChecked() || ui->secondSelectBox->isChecked() ||
         ui->thirdSelectBox->isChecked() || ui->fourthSelectBox->isChecked()) &&
        (ui->trueButton->isChecked() || ui->falseButton->isChecked())) {

        // Adding mult choice to vector userAnswers
        if (ui->firstMultButton->isChecked()) {
            userAnswers.push_back(ui->firstMultButton->text());
        } else if (ui->secondMultButton->isChecked()) {
            userAnswers.push_back(ui->secondMultButton->text());
        } else if (ui->thirdMultButton->isChecked()) {
            userAnswers.push_back(ui->thirdMultButton->text());
        } else if (ui->fourthMultButton->isChecked()) {
            userAnswers.push_back(ui->fourthMultButton->text());
        }

        // Adding true false answer to vector of user answers
        if (ui->trueButton->isChecked()) {
            userAnswers.push_back(ui->trueButton->text());
        } else if (ui->falseButton->isChecked()) {
            userAnswers.push_back(ui->falseButton->text());
        }

        // Selections to userAnswers vector
        if (ui->firstSelectBox->isChecked()) {
            userAnswers.push_back(ui->firstSelectBox->text());
        }
        if (ui->secondSelectBox->isChecked()) {
            userAnswers.push_back(ui->secondSelectBox->text());
        }
        if (ui->thirdSelectBox->isChecked()) {
            userAnswers.push_back(ui->thirdSelectBox->text());
        }
        if (ui->fourthSelectBox->isChecked()) {
            userAnswers.push_back(ui->fourthSelectBox->text());
        }
    }

    // emit some signal letting the model know the answers have been collected
    // and put into userAnswers
    emit answersCollected(userAnswers);
    resetSelections();
    userAnswers.clear();
}

void Quiz::submitCancelClicked() {
    resetSelections();
    emit closeWindow();
}

void Quiz::quizGradedUpdateWindow(bool passed) {
    if (passed) {
        // change label to passed
        ui->label->setStyleSheet(
            "font: 18pt; font-weight:bold; font-family: 'Helvetica'; "
            "border-style: outset; border-width: 4px; min-width: 10em; "
            "border-color: black; background-color: rgb(132, 240, 115); "
            "padding: "
            "6px;");
        ui->label->setText("Quiz passed! You earned a badge!");

        ui->label->setVisible(true);
        QTimer::singleShot(1500, this, &Quiz::hidePassedLabel);
    } else {
        // change label to failed
        ui->label->setStyleSheet(
            "font: 22pt; font-weight:bold; font-family: 'Helvetica'; "
            "border-style: outset; border-width: 4px; min-width: 10em; "
            "border-color: black; background-color: rgb(237, 109, 111); "
            "padding: "
            "6px;");
        ui->label->setText("Quiz failed! Try again!");
        ui->label->setVisible(true);
        QTimer::singleShot(1500, this, &Quiz::hideFailedLabel);
    }
}

void Quiz::hideFailedLabel() {
    ui->label->setVisible(false);
}

void Quiz::hidePassedLabel() {
    ui->label->setVisible(false);
    emit closeWindow();
}
void Quiz::resetSelections() {
    // True False buttons
    ui->falseButton->setAutoExclusive(false);
    ui->falseButton->setChecked(false);
    ui->falseButton->setAutoExclusive(true);
    ui->trueButton->setAutoExclusive(false);
    ui->trueButton->setChecked(false);
    ui->trueButton->setAutoExclusive(true);

    // Mult choice buttons
    ui->firstMultButton->setAutoExclusive(false);
    ui->firstMultButton->setChecked(false);
    ui->firstMultButton->setAutoExclusive(true);
    ui->secondMultButton->setAutoExclusive(false);
    ui->secondMultButton->setChecked(false);
    ui->secondMultButton->setAutoExclusive(true);
    ui->thirdMultButton->setAutoExclusive(false);
    ui->thirdMultButton->setChecked(false);
    ui->thirdMultButton->setAutoExclusive(true);
    ui->fourthMultButton->setAutoExclusive(false);
    ui->fourthMultButton->setChecked(false);
    ui->fourthMultButton->setAutoExclusive(true);

    // Select all buttons
    ui->firstSelectBox->setChecked(false);
    ui->secondSelectBox->setChecked(false);
    ui->thirdSelectBox->setChecked(false);
    ui->fourthSelectBox->setChecked(false);
}
