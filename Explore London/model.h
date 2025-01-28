/*
 * The header file for the model.
 * This class contains state and functions to modify state for most systems in
 * the application. This includes physics, quizzes, ui, badges, map, etc.
 * Authors: Chase Yates, David Meredith, Brandy Cervantes
 * Reviewed by: Melissa Umble, Brandy Cervantes
 * Date: 4/24/2024
 */

#ifndef MODEL_H
#define MODEL_H

#include "location.h"
#include "mapexplorer.h"
#include "questionsanswers.h"

#include <Box2D/Box2D.h>
#include <QIcon>
#include <QMap>
#include <QObject>
#include <QString>
#include <QTimer>

#include <map>

class Model : public QObject {
    Q_OBJECT

public:
    /// @brief constructs the model with the specified parent. Used by QT
    /// @param parent The QT object that is parent of this model
    explicit Model(QObject* parent = nullptr);

    /// @brief destroys the model and deletes dangling data
    ~Model();

    /// @brief adds all location pins to the specified map
    /// @param map A pointer to the map explorer to add the pins to
    void addPinsToMap(MapExplorer* map);

    /// @brief gets the quiz info from the button with the specified name and
    /// emits a signal to pop up the corresponding quiz
    /// @param buttonName the name of the quiz button
    void getQuizInfo(QString buttonName);

    /// @brief returns whether the badge of the specified type is unlocked or
    /// not
    /// @param type The type of badge to check
    bool isBadgeUnlocked(BadgeType type);

    /// @brief initializes the physics for Box2D
    void initBox2D();

signals:
    /// @brief emitted for updating window popups
    /// @param location a reference to location info for the popup
    void updateViewPopup(const Location& location);

    /// @brief emitted for updating quiz window popup
    /// @param quizInfo the QuestionsAnswers data structure holding all quiz
    /// info
    void UpdateViewQuizPopup(QuestionsAnswers quizInfo);

    /// @brief emitted when a quiz is completed, provides info on whether the
    /// quiz was passed or not, and the type of quiz
    /// @param passed Whether the quiz was passed(true) or failed(false)
    /// @param type The type of quiz
    void quizResults(bool passed, BadgeType type);

    /// @brief emitted to enable the unlocked badge icon
    /// @param type The type of badge to enable the icon of
    void enableBadgeIcon(BadgeType type);

    /// @brief emitted for the map explorer to add the physics badge to its view
    /// @param type The type of badge
    /// @param phys The Box2D object handle for the badge
    /// @param icon The badge's icon for the map explorer to handle
    void addBadgeToView(BadgeType type, b2Body* phys, QIcon icon);

    /// @brief emitted for the map explorer to delete a physics badge
    /// @param type the type of badge to remove (there should only ever be 1 at
    /// a time)
    void removeBadgeFromView(BadgeType type);

    /// @brief emitted for the map to redraw
    void redrawMap();

    /// @brief emitted to disable the quiz button for a quiz (used when a quiz
    /// is passed and does not need to be taken anymore
    /// @param quizName the name of the quiz to disable
    void disableQuizButton(QString quizName);

    /// @brief emitted when all badges have been unlocked
    void allBadgesUnlocked();

public slots:
    /// @brief checks whether the provided answers to the current active quiz
    /// are correct
    /// @param userAnswers the answers the user provided to check
    void checkQuizAnswers(QVector<QString> userAnswers);

    /// @brief unlocks the specified badge type if the quiz was passed
    /// @param quizPassed whether the quiz was passed
    /// @param type The type of Badge to potentially unlock
    void unlockBadge(bool quizPassed, BadgeType type);

    /// @brief Toggles whether the physics bridge badge is on the map explorer
    void toggleBridgeBadgeRequested();

    /// @brief Toggles whether the physics art and culture badge is on the map
    /// explorer
    void toggleArtCultureBadgeRequested();

    /// @brief Toggles whether the physics crimes badge is on the map explorer
    void toggleCrimesBadgeRequested();

    /// @brief Toggles whether the physics government badge is on the map
    /// explorer
    void toggleGovernmentBadgeRequested();

    /// @brief Toggles whether the physics religion badge is on the map explorer
    void toggleReligionBadgeRequested();

    /// @brief Toggles whether the physics royal family badge is on the map
    /// explorer
    void toggleRoyalFamilyBadgeRequested();

    /// @brief updates Box2D physics
    void updatePhysics();

private:
    QMap<QString, Location> locationInformation;
    QMap<QString, QuestionsAnswers> quizInformation;

    /// @brief reads the quiz text file provided and returns its parsed info
    /// @param txtFilePath the file path to the quiz file
    QMap<QString, QuestionsAnswers> readQuizTextFile(QString txtFilePath);

    /// @brief reads the provided location text file and provides its info
    /// @param txtFilePath path to the locations file
    QMap<QString, Location> readTextFile(QString txtFilePath);
    QString currTheme;

    bool bridgeBadgeUnlocked = false;
    bool artCultureBadgeUnlocked = false;
    bool crimesBadgeUnlocked = false;
    bool governmentBadgeUnlocked = false;
    bool religionBadgeUnlocked = false;
    bool royalFamilyBadgeUnlocked = false;

    // track when physics badges should be enabled
    bool bridgeBadgeEnabled = false;
    bool artCultureBadgeEnabled = false;
    bool crimesBadgeEnabled = false;
    bool governmentBadgeEnabled = false;
    bool religionBadgeEnabled = false;
    bool royalFamilyBadgeEnabled = false;

    /// @brief local helper function for toggling physics badges
    /// @param type The type of badge to toggle
    void toggleBadge(BadgeType type);

    b2World world;
    b2Body* downWall;
    b2Body* upWall;
    b2Body* rightWall;
    b2Body* leftWall;

    std::map<BadgeType, b2Body*> bodies;

    QTimer physicsTimer;
};

#endif // MODEL_H
