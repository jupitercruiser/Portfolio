#ifndef CELEBRATIONSCREEN_H
#define CELEBRATIONSCREEN_H
#include <QDialog>
#include <QPoint>
#include <QVector>

/// The .h file for a celebration screen ui window that displays
/// a celevration after you have collected all the badges
///
/// Author: Brady Spinti
/// Reviewed by: David Meredith
/// Date: April 24, 2024
///

namespace Ui {
class CelebrationScreen;
}

class CelebrationScreen : public QDialog {
    Q_OBJECT

public:
    /// @brief constructor for the celebration screen
    explicit CelebrationScreen(QWidget* parent = nullptr);

    /// @brief destructor for the celebration screen
    ~CelebrationScreen();

private:
    Ui::CelebrationScreen* ui;

    /// @brief keeps track of when to draw new balloons
    int nextBalloonCount;

    /// @brief keeps track of when to draw new explosions
    int nextExplosionCount;

    /// @brief vector that holds the position and movement of the balloons
    QVector<QPair<QPoint, int>> balloons;

    /// @brief vector that holds the position and current phase of the
    /// explosions
    QVector<QPair<QPoint, int>> explosions;

    /// @brief vector that holds the position and color of the confettis
    QVector<QPair<QPoint, QColor>> confettis;

    /// @brief each is an image that is part of the explosions
    QPixmap explosion0;
    QPixmap explosion1;
    QPixmap explosion2;
    QPixmap explosion3;
    QPixmap explosion4;

    /// @brief holds background image
    QPixmap backgroundImage;

    /// @brief adds a balloon to the ballons vector with a random position on
    /// the screen
    void addBalloon();

    /// @brief adds a explosion to the explosions vector with a random position
    /// on the screen
    void addExplosion();

    /// @brief adds a confetti piece to the confettis vector with a random
    /// position on the screen and random color
    void addConfetti();

protected:
    /// @brief called repitively to update and repaint the elements of the
    /// celebration on the screen so that it appears that they are moving
    void paintEvent(QPaintEvent*);
};

#endif // CELEBRATIONSCREEN_H
