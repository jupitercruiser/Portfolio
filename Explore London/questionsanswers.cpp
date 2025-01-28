/// The .cpp file for a QuestionsAnswers class that fills in the
/// necessary information for a quiz
///
/// Authors: Melissa Umble and Brandy Cervantes
/// Reviewed by: Seth Brown
/// Date: April 24, 2024

#include "questionsanswers.h"
#include <QMap>
#include <QRandomGenerator>
#include <QString>

QuestionsAnswers::QuestionsAnswers() {}

QuestionsAnswers::QuestionsAnswers(
    QString theme, QString multQuestion, QString selectAllQuestion,
    QString trueFalseQuestion, QString multAnswerA, QString multAnswerB,
    QString multAnswerC, QString multAnswerD, QString selectAllAnswerA,
    QString selectAllAnswerB, QString selectAllAnswerC,
    QString selectAllAnswerD, QString trueFalseAnswerA,
    QString trueFalseAnswerB, QString multChoiceCorrectAnswer,
    std::vector<QString> selectAllCorrectAnswers,
    QString trueFalseCorrectAnswer) {

    // theme
    this->theme = theme;

    // questions
    this->multQuestion = multQuestion;
    this->selectAllQuestion = selectAllQuestion;
    this->trueFalseQuestion = trueFalseQuestion;

    // mult options
    this->multAnswerA = multAnswerA;
    this->multAnswerB = multAnswerB;
    this->multAnswerC = multAnswerC;
    this->multAnswerD = multAnswerD;

    // select all options
    this->selectAllAnswerA = selectAllAnswerA;
    this->selectAllAnswerB = selectAllAnswerB;
    this->selectAllAnswerC = selectAllAnswerC;
    this->selectAllAnswerD = selectAllAnswerD;

    // true false options
    this->trueFalseAnswerA = trueFalseAnswerA;
    this->trueFalseAnswerB = trueFalseAnswerB;

    // correct answers
    this->multChoiceCorrectAnswer = multChoiceCorrectAnswer;
    this->selectAllCorrectAnswers = selectAllCorrectAnswers;
    this->trueFalseCorrectAnswer = trueFalseCorrectAnswer;

    QString multOptions[]{multAnswerA, multAnswerB, multAnswerC, multAnswerD};
    QString selectAllOptions[]{selectAllAnswerA, selectAllAnswerB,
                               selectAllAnswerC, selectAllAnswerD};
    QString trueFalseOptions[]{trueFalseAnswerA, trueFalseAnswerB};
}
