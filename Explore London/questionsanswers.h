/// The .h file for the QuestionsAnswers class for ExploreLondon
///
/// Holds the necessary information for a quiz, its answer options, and its
/// correct answers
///
/// Authors: Brandy Cervantes and Melissa Umble
/// Reviewed by: Seth Brown
/// Date: April 24, 2024

#ifndef QUESTIONSANSWERS_H
#define QUESTIONSANSWERS_H

#include <QMap>
#include <QRandomGenerator>
#include <QString>

/// @brief The QuestionsAnswers class contains necessary information
///         for a quiz
class QuestionsAnswers {
public:
    ///@brief default constructor
    QuestionsAnswers();

    ///@brief Quiz constructor with information from quiz text file
    /// @param theme quiz theme
    /// @param multQuestion multiple choice question
    /// @param selectAllQuestion
    /// @param trueFalseQuestion
    /// @param multAnswerA multiple choice answer
    /// @param multAnswerB multiple choice answer
    /// @param multAnswerC multiple choice answer
    /// @param multAnswerD multiple choice answer
    /// @param selectAllAnswerA a select all answer
    /// @param selectAllAnswerB a select all answer
    /// @param selectAllAnswerC a select all answer
    /// @param selectAllAnswerD a select all answer
    /// @param trueFalseAnswerA a true or false choice
    /// @param trueFalseAnswerB a true or false choice
    /// @param multChoiceCorrectAnswer the correct multiple choice answer
    /// @param selectAllCorrectAnswers the correct select all answers contained
    /// in a list
    /// @param trueFalseCorrectAnswer the correct true or false question answer
    QuestionsAnswers(QString theme, QString multQuestion,
                     QString selectAllQuestion, QString trueFalseQuestion,
                     QString multAnswerA, QString multAnswerB,
                     QString multAnswerC, QString multAnswerD,
                     QString selectAllAnswerA, QString selectAllAnswerB,
                     QString selectAllAnswerC, QString selectAllAnswerD,
                     QString trueFalseAnswerA, QString trueFalseAnswerB,
                     QString multChoiceCorrectAnswer,
                     std::vector<QString> selectAllCorrectAnswers,
                     QString trueFalseCorrectAnswer);

    QString theme;
    QString multQuestion;
    QString selectAllQuestion;
    QString trueFalseQuestion;

    QString multAnswerA;
    QString multAnswerB;
    QString multAnswerC;
    QString multAnswerD;

    QString selectAllAnswerA;
    QString selectAllAnswerB;
    QString selectAllAnswerC;
    QString selectAllAnswerD;

    QString trueFalseAnswerA;
    QString trueFalseAnswerB;

    QString multChoiceCorrectAnswer;
    std::vector<QString> selectAllCorrectAnswers;
    QString trueFalseCorrectAnswer;
};

#endif // QUESTIONSANSWERS_H
