/*
 * This is the implementation of the model class.
 * It modifies application state for various systems such as physics, badges,
 * quizzes, map, ui, etc.
 * Authors: Chase Yates, David Meredith, Brandy Cervantes
 * Reviewed by: Melissa Umble
 * Date: 4/24/2024
 */

#include "model.h"
#include "location.h"
#include <QDebug>
#include <QFile>
#include <QImage>
#include <QString>
#include <QTextStream>
#include <QVector>
#include <algorithm>

Model::Model(QObject* parent)
    : QObject(parent), world(b2Vec2(0.0f, 0.0f)), physicsTimer() {
    locationInformation = this->readTextFile(":/textInformation");
    quizInformation = this->readQuizTextFile(":/textQuiz");
}

Model::~Model() {}

void Model::addPinsToMap(MapExplorer* map) {
    // by placing the pins on the map from top to bottom, we prevent a pin
    // that's farther up the map from being on top of a lower one, which looks
    // weird
    QVector<const Location*> sortedLocations;
    for (const Location& location : std::as_const(locationInformation)) {
        sortedLocations.push_back(&location);
    }
    std::sort(
        sortedLocations.begin(), sortedLocations.end(),
        [](const Location* a, const Location* b) { return a->pinY < b->pinY; });

    for (const Location* location : sortedLocations) {
        map->addPin(*location,
                    [this, location] { emit updateViewPopup(*location); });
    }
}

void Model::initBox2D() {
    // world and timer initialized in constructor, complete initalization here:
    connect(&physicsTimer, &QTimer::timeout, this, &Model::updatePhysics);

    // get explorer info
    int width = 5750;
    int height = 3850;
    int xOffset = width / 2;
    int yOffset = height / 2;

    // create borders

    // down border
    b2BodyDef downDef;
    downDef.position.Set(xOffset, -height / 2 - 1 + yOffset);
    b2PolygonShape downShape;
    downShape.SetAsBox(width, 2);
    downWall = world.CreateBody(&downDef);
    b2FixtureDef downFixture;
    downFixture.density = 1.0f;
    downFixture.friction = 0.0f;
    downFixture.restitution = 1.0f;
    downFixture.shape = &downShape;
    downWall->CreateFixture(&downFixture);

    // up border
    b2BodyDef upDef;
    upDef.position.Set(xOffset, height / 2 + 1 + yOffset);
    b2PolygonShape upShape;
    upShape.SetAsBox(width, 2);
    upWall = world.CreateBody(&upDef);
    b2FixtureDef upFixture;
    upFixture.density = 1.0f;
    upFixture.friction = 0.0f;
    upFixture.restitution = 1.0f;
    upFixture.shape = &upShape;
    upWall->CreateFixture(&upFixture);

    // right border
    b2BodyDef rightDef;
    rightDef.position.Set(width / 2 + 1 + xOffset, yOffset);
    b2PolygonShape rightShape;
    rightShape.SetAsBox(2, height);
    rightWall = world.CreateBody(&rightDef);
    b2FixtureDef rightFixture;
    rightFixture.density = 1.0f;
    rightFixture.friction = 0.0f;
    rightFixture.restitution = 1.0f;
    rightFixture.shape = &rightShape;
    rightWall->CreateFixture(&rightFixture);

    // left border
    b2BodyDef leftDef;
    leftDef.position.Set(-width / 2 - 1 + xOffset, yOffset);
    b2PolygonShape leftShape;
    leftShape.SetAsBox(2, height);
    leftWall = world.CreateBody(&leftDef);
    b2FixtureDef leftFixture;
    leftFixture.density = 1.0f;
    leftFixture.friction = 0.0f;
    leftFixture.restitution = 1.0f;
    leftFixture.shape = &leftShape;
    leftWall->CreateFixture(&leftFixture);

    physicsTimer.start(17); // 17 milliseconds is roughly 1/60th of a second
}

void Model::updatePhysics() {
    world.Step(0.017f, 6, 2);
    emit redrawMap();
}

QMap<QString, Location> Model::readTextFile(QString txtFilePath) {
    QMap<QString, Location> locations;
    QFile file(txtFilePath);
    file.open(QIODevice::ReadOnly);

    QTextStream reader(&file);
    QString line;

    QString title;
    QString theme;
    QString information;
    QString pictureFilepath;
    QString iconFilepath;
    qreal pinX;
    qreal pinY;

    while (!reader.atEnd()) {
        try {
            // the beginning of the loop
            line = reader.readLine();
            title = line;

            line = reader.readLine();
            theme = line;

            // description can be multiline, so go to end of line
            information.clear(); // clear from last loop
            line = reader.readLine();
            while (line.size() != 0) {
                information.append(line);
                line = reader.readLine();
            }

            // now at empty line
            line = reader.readLine();
            pictureFilepath = line;

            line = reader.readLine();
            iconFilepath = line;

            line = reader.readLine();
            pinX = line.toDouble();

            line = reader.readLine();
            pinY = line.toDouble();

            // now move to next line to restart loop
            line = reader.readLine();
            line = reader.readLine();

            // create location and add to map
            Location location(title, theme, information, pictureFilepath,
                              iconFilepath, pinX, pinY);
            locations.insert(title, location);

        } catch (...) {
            qDebug("Problem reading data in file, make sure an empty line is "
                   "at the end of the text file");
        }
    }

    file.close();

    return locations;
}

QMap<QString, QuestionsAnswers> Model::readQuizTextFile(QString txtFilePath) {

    QMap<QString, QuestionsAnswers> questionsAndAnswers;
    QFile file(txtFilePath);
    file.open(QIODevice::ReadOnly);

    QTextStream reader(&file);
    QString line;

    QString theme;
    QString multChoiceQuestion;
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

    // CORRECT ANSWERS STORED IN THE FOLLOWING:
    QString multChoiceCorrectAnswer;
    std::vector<QString> selectAllCorrectAnswers;
    QString trueFalseCorrectAnswer;

    while (!reader.atEnd()) {
        try {
            // the beginning of the loop

            // READ THEME
            line = reader.readLine();
            theme = line;

            // READ QUESTIONS
            // question 1 can be multiline, so go to end of line
            multChoiceQuestion.clear(); // clear from last loop
            line = reader.readLine();
            while (line.size() != 0) {
                multChoiceQuestion.append(line);
                line = reader.readLine();
            }

            // question 2 can be multiline, so go to end of line
            selectAllQuestion.clear(); // clear from last loop
            line = reader.readLine();
            while (line.size() != 0) {
                selectAllQuestion.append(line);
                line = reader.readLine();
            }

            // question 3 can be multiline, so go to end of line
            trueFalseQuestion.clear(); // clear from last loop
            line = reader.readLine();
            while (line.size() != 0) {
                trueFalseQuestion.append(line);
                line = reader.readLine();
            }

            // READ MULT CHOICE OPTIONS
            // read multiple choice answers
            line = reader.readLine();
            multAnswerA = line;
            multChoiceCorrectAnswer = line;

            line = reader.readLine();
            multAnswerB = line;

            line = reader.readLine();
            multAnswerC = line;

            line = reader.readLine();
            multAnswerD = line;

            // empty line
            line = reader.readLine();

            // READ SELECT ALL OPTIONS
            // read select all answers
            selectAllCorrectAnswers.clear(); // clear from last loop

            // read Answer A
            line = reader.readLine();
            if (line == "Right") {
                line = reader.readLine();
                selectAllAnswerA = line;
                selectAllCorrectAnswers.push_back(line);
            } else if (line == "Wrong") {
                line = reader.readLine();
                selectAllAnswerA = line;
            }

            // read Answer B
            line = reader.readLine();
            if (line == "Right") {
                line = reader.readLine();
                selectAllAnswerB = line;
                selectAllCorrectAnswers.push_back(line);
            } else if (line == "Wrong") {
                line = reader.readLine();
                selectAllAnswerB = line;
            }

            // read Answer C
            line = reader.readLine();
            if (line == "Right") {
                line = reader.readLine();
                selectAllAnswerC = line;
                selectAllCorrectAnswers.push_back(line);
            } else if (line == "Wrong") {
                line = reader.readLine();
                selectAllAnswerC = line;
            }

            // read Answer D
            line = reader.readLine();
            if (line == "Right") {
                line = reader.readLine();
                selectAllAnswerD = line;
                selectAllCorrectAnswers.push_back(line);
            } else if (line == "Wrong") {
                line = reader.readLine();
                selectAllAnswerD = line;
            }

            // empty line
            line = reader.readLine();

            // READ TRUE FALSE OPTIONS
            // read true or false answers
            line = reader.readLine();
            trueFalseAnswerA = line;
            trueFalseCorrectAnswer = line;

            line = reader.readLine();
            trueFalseAnswerB = line;

            // now move to next line to restart loop
            line = reader.readLine();
            line = reader.readLine();

            QuestionsAnswers qAndA(
                theme, multChoiceQuestion, selectAllQuestion, trueFalseQuestion,
                multAnswerA, multAnswerB, multAnswerC, multAnswerD,
                selectAllAnswerA, selectAllAnswerB, selectAllAnswerC,
                selectAllAnswerD, trueFalseAnswerA, trueFalseAnswerB,
                multChoiceCorrectAnswer, selectAllCorrectAnswers,
                trueFalseCorrectAnswer);

            questionsAndAnswers.insert(theme, qAndA);

        } catch (...) {
            qDebug("Problem reading data in file, make sure an empty line is "
                   "at the end of the text file");
        }
    }

    file.close();
    return questionsAndAnswers;
}

void Model::getQuizInfo(QString buttonName) {
    if (buttonName == "royalFamQuizButton") {
        currTheme = "Royal Family";
        emit UpdateViewQuizPopup(quizInformation["Royal Family"]);

    } else if (buttonName == "artsCultureQuizButton") {
        currTheme = "Arts and Culture";
        emit UpdateViewQuizPopup(quizInformation["Arts and Culture"]);

    } else if (buttonName == "crimesQuizButton") {
        currTheme = "Crime";
        emit UpdateViewQuizPopup(quizInformation["Crime"]);

    } else if (buttonName == "bridgesQuizButton") {
        currTheme = "Bridges";
        emit UpdateViewQuizPopup(quizInformation["Bridges"]);

    } else if (buttonName == "governmentQuizButton") {
        currTheme = "Government";
        emit UpdateViewQuizPopup(quizInformation["Government"]);

    } else {
        currTheme = "Religion";
        emit UpdateViewQuizPopup(quizInformation["Religion"]);
    }
}

void Model::checkQuizAnswers(QVector<QString> userAnswers) {
    QuestionsAnswers qAndA = quizInformation.find(currTheme).value();

    BadgeType type;
    if (qAndA.theme == "Royal Family") {
        type = BadgeType::RoyalFamily;
    } else if (qAndA.theme == "Arts and Culture") {
        type = BadgeType::ArtAndCulture;
    } else if (qAndA.theme == "Religion") {
        type = BadgeType::Religion;
    } else if (qAndA.theme == "Government") {
        type = BadgeType::Government;
    } else if (qAndA.theme == "Crime") {
        type = BadgeType::Crimes;
    } else {
        type = BadgeType::Bridges;
    }

    if (userAnswers.size() < 2) {
        emit quizResults(false, type);
        return;
    }
    if (userAnswers[0] != qAndA.multChoiceCorrectAnswer) {
        emit quizResults(false, type);
        return;
    } else if (userAnswers[1] != qAndA.trueFalseCorrectAnswer) {
        // emit a signal saying the user did not pass
        emit quizResults(false, type);
        return;
    }
    if (static_cast<size_t>(userAnswers.size() - 2) !=
        qAndA.selectAllCorrectAnswers.size()) {
        emit quizResults(false, type);
    }

    for (unsigned long i = 0; i < qAndA.selectAllCorrectAnswers.size(); i++) {
        int cnt =
            count(qAndA.selectAllCorrectAnswers.begin(),
                  qAndA.selectAllCorrectAnswers.end(), userAnswers[i + 2]);
        if (cnt == 0) { // user answer not found in correct answers
            // emit a signal saying the user did not pass
            emit quizResults(false, type);
            return;
        }
    }

    // emit some signal determining whether or not the user passed the quiz
    emit quizResults(true, type);
}

void Model::unlockBadge(bool quizPassed, BadgeType type) {
    if (!quizPassed) {
        return;
    }
    // enable icon
    emit enableBadgeIcon(type);

    // update model
    switch (type) {
    case BadgeType::Bridges:
        bridgeBadgeUnlocked = true;
        // disable quiz on main window since it was taken and passed
        // emit disableQuizButton(QString quizName)
        emit disableQuizButton("bridges");
        break;
    case BadgeType::ArtAndCulture:
        artCultureBadgeUnlocked = true;
        emit disableQuizButton("art and culture");
        break;
    case BadgeType::Crimes:
        crimesBadgeUnlocked = true;
        emit disableQuizButton("crimes");
        break;
    case BadgeType::Government:
        governmentBadgeUnlocked = true;
        emit disableQuizButton("government");
        break;
    case BadgeType::Religion:
        religionBadgeUnlocked = true;
        emit disableQuizButton("religion");
        break;
    case BadgeType::RoyalFamily:
        royalFamilyBadgeUnlocked = true;
        emit disableQuizButton("royal family");
        break;
    }
    if (bridgeBadgeUnlocked && artCultureBadgeUnlocked && crimesBadgeUnlocked &&
        governmentBadgeUnlocked && religionBadgeUnlocked &&
        royalFamilyBadgeUnlocked) {
        emit allBadgesUnlocked();
    }
}

bool Model::isBadgeUnlocked(BadgeType type) {
    switch (type) {
    case BadgeType::Bridges:
        return bridgeBadgeUnlocked;
    case BadgeType::ArtAndCulture:
        return artCultureBadgeUnlocked;
    case BadgeType::Crimes:
        return crimesBadgeUnlocked;
    case BadgeType::Government:
        return governmentBadgeUnlocked;
    case BadgeType::Religion:
        return religionBadgeUnlocked;
    case BadgeType::RoyalFamily:
        return royalFamilyBadgeUnlocked;
    }
    return false;
}

void Model::toggleBridgeBadgeRequested() {
    toggleBadge(BadgeType::Bridges);
}

void Model::toggleArtCultureBadgeRequested() {
    toggleBadge(BadgeType::ArtAndCulture);
}

void Model::toggleCrimesBadgeRequested() {
    toggleBadge(BadgeType::Crimes);
}

void Model::toggleGovernmentBadgeRequested() {
    toggleBadge(BadgeType::Government);
}

void Model::toggleReligionBadgeRequested() {
    toggleBadge(BadgeType::Religion);
}

void Model::toggleRoyalFamilyBadgeRequested() {
    toggleBadge(BadgeType::RoyalFamily);
}

void Model::toggleBadge(BadgeType type) {
    if (!isBadgeUnlocked(type)) {
        return;
    }

    bool* toToggle = nullptr;
    QString iconPath;
    switch (type) {
    case BadgeType::Bridges:
        toToggle = &bridgeBadgeEnabled;
        iconPath = ":/icons/bridges.png";
        break;
    case BadgeType::ArtAndCulture:
        toToggle = &artCultureBadgeEnabled;
        iconPath = ":/icons/art_culture.png";
        break;
    case BadgeType::Crimes:
        toToggle = &crimesBadgeEnabled;
        iconPath = ":/icons/crimes.png";
        break;
    case BadgeType::Government:
        toToggle = &governmentBadgeEnabled;
        iconPath = ":/icons/government.png";
        break;
    case BadgeType::Religion:
        toToggle = &religionBadgeEnabled;
        iconPath = ":/icons/religion.png";
        break;
    case BadgeType::RoyalFamily:
        toToggle = &royalFamilyBadgeEnabled;
        iconPath = ":/icons/royal_family.png";
        break;
    }
    *toToggle = !*toToggle;
    if (*toToggle) {
        // spawn new physics body with icon

        // create physics body for badge
        b2BodyDef badgeDef;
        badgeDef.type = b2_dynamicBody;
        badgeDef.position.Set(3000.0f, 2000.0f);
        b2PolygonShape badgeShape;
        badgeShape.SetAsBox(32.0f, 32.0f);
        b2FixtureDef badgeFixture;
        badgeFixture.shape = &badgeShape;
        badgeFixture.density = 0.0001f;
        badgeFixture.friction = 0.0f;
        b2Body* badgeBody = world.CreateBody(&badgeDef);
        badgeBody->CreateFixture(&badgeFixture);
        badgeFixture.restitution = 1.0f;
        float maxVel = 500000000;
        float randomX =
            (((float)rand()) / (float)RAND_MAX) * 2 * maxVel - maxVel;
        float randomY =
            (((float)rand()) / (float)RAND_MAX) * 2 * maxVel - maxVel;

        badgeBody->ApplyForceToCenter(b2Vec2(randomX, randomY), true);
        bodies[type] = badgeBody;
        emit addBadgeToView(type, badgeBody, QIcon(iconPath));
        return;
    }

    if (bodies.count(type) <= 0) {
        return;
    }

    // otherwise delete associated physics body

    world.DestroyBody(bodies[type]);
    emit removeBadgeFromView(type);
}
