#include "celebrationscreen.h"
#include "ui_celebrationscreen.h"
#include <QPainter>
#include <QPen>
#include <QRandomGenerator>
#include <QTimer>

/// The .cpp file for a celebration screen ui window that implements
/// and draws the celebration
///
/// Author: Brady Spinti
/// Reviewed by: David Meredith
/// Date: April 24, 2024

CelebrationScreen::CelebrationScreen(QWidget* parent)
    : QDialog(parent), ui(new Ui::CelebrationScreen) {
    ui->setupUi(this);

    backgroundImage = QPixmap(":/images/background.png");
    backgroundImage = backgroundImage.scaled(QSize(660, 472));

    nextBalloonCount = 0;
    nextExplosionCount = 0;

    // draw you win image on screen
    QPixmap youWinImage(":/images/youWin2.png");
    youWinImage = youWinImage.scaled(QSize(350, 300));
    ui->youWinLabel->setPixmap(youWinImage);

    // create the pixmaps for the explosion animation
    explosion0 = QPixmap(":/images/Explosion1.png");
    explosion0 = explosion0.scaled(QSize(100, 100), Qt::KeepAspectRatio);
    explosion1 = QPixmap(":/images/Explosion2.png");
    explosion1 = explosion1.scaled(QSize(100, 100), Qt::KeepAspectRatio);
    explosion2 = QPixmap(":/images/Explosion.png");
    explosion2 = explosion2.scaled(QSize(100, 100), Qt::KeepAspectRatio);
    explosion3 = QPixmap(":/images/Explosion3.png");
    explosion3 = explosion3.scaled(QSize(100, 100), Qt::KeepAspectRatio);
    explosion4 = QPixmap(":/images/Explosion4.png");
    explosion4 = explosion4.scaled(QSize(100, 100), Qt::KeepAspectRatio);
}

void CelebrationScreen::paintEvent(QPaintEvent*) {
    QPainter painter(this);

    // draws british flag on the screen
    QRect backgroundSize(0, 0, 660, 472);
    painter.drawPixmap(backgroundSize, backgroundImage, backgroundSize);

    QPen pen(Qt::NoPen);
    painter.setPen(pen);
    painter.setBrush(Qt::red);

    // adds in elements in the celebration
    if (balloons.size() < 7) {
        addBalloon();
    }

    if (explosions.size() < 3) {
        addExplosion();
    }

    if (confettis.size() < 100) {
        addConfetti();
    }

    // udpates and draws balloons
    for (auto& balloonPair : balloons) {
        auto& balloon = balloonPair.first;
        painter.setBrush(Qt::red);

        // moves balloon up
        balloon.setY(balloon.y() - 5);

        // moves balloon side to side
        if (balloonPair.second == 0) {
            int randomMovement = QRandomGenerator::global()->bounded(-10, 11);
            if (balloonPair.second > 0) {
                balloonPair.second = randomMovement + 5;
            } else {
                balloonPair.second = randomMovement - 5;
            }
        } else if (balloonPair.second > 0) {
            balloon.setX(balloon.x() + 2);
            balloonPair.second--;
        } else {
            balloon.setX(balloon.x() - 2);
            balloonPair.second++;
        }

        // draws balloon
        painter.drawEllipse(balloon.x(), balloon.y(), 28, 44);
        painter.drawEllipse(balloon.x() + 4, balloon.y() + 16, 20, 36);

        QPen pen(Qt::black);
        painter.setPen(pen);
        painter.drawLine(balloon.x() + 14, balloon.y() + 52, balloon.x() + 14,
                         balloon.y() + 100);

        QPen pen2(Qt::NoPen);
        painter.setPen(pen2);
        painter.setBrush(Qt::white);
        painter.drawEllipse(balloon.x() + 9, balloon.y() + 15, 5, 5);

        if (balloon.y() < -100) {
            balloons.remove(balloons.indexOf(balloonPair));
        }
    }

    // udpates and draws explosions
    for (auto& explosionPair : explosions) {
        auto& explosion = explosionPair.first;

        // draws current phase of the explosion
        if (explosionPair.second <= 2) {
            QRect explosionLocation(explosion.x(), explosion.y(), 100, 100);
            QRect explosionSize(0, 0, 100, 100);
            painter.drawPixmap(explosionLocation, explosion0, explosionSize);
        } else if (explosionPair.second <= 6) {
            QRect explosionLocation(explosion.x(), explosion.y(), 100, 100);
            QRect explosionSize(0, 0, 100, 100);
            painter.drawPixmap(explosionLocation, explosion1, explosionSize);
        } else if (explosionPair.second <= 10) {
            QRect explosionLocation(explosion.x(), explosion.y(), 100, 100);
            QRect explosionSize(0, 0, 100, 100);
            painter.drawPixmap(explosionLocation, explosion2, explosionSize);
        } else if (explosionPair.second <= 13) {
            QRect explosionLocation(explosion.x(), explosion.y(), 100, 100);
            QRect explosionSize(0, 0, 100, 100);
            painter.drawPixmap(explosionLocation, explosion3, explosionSize);
        } else if (explosionPair.second <= 16) {
            QRect explosionLocation(explosion.x(), explosion.y(), 100, 100);
            QRect explosionSize(0, 0, 100, 100);
            painter.drawPixmap(explosionLocation, explosion4, explosionSize);
        } else {
            explosions.remove(explosions.indexOf(explosionPair));
        }

        explosionPair.second++;
    }

    // udpates and draws confetti
    for (auto& confettiPair : confettis) {
        painter.setBrush(confettiPair.second);
        auto& confetti = confettiPair.first;

        int randomPosition = QRandomGenerator::global()->bounded(0, 3);

        // updates confetti position
        confetti.setY(confetti.y() + 10);

        // based on random position draws the confetti in different ways so
        // looks like it is falling
        if (randomPosition == 0) {
            painter.drawRect(QRect(confetti.x(), confetti.y(), 10, 2));
        } else if (randomPosition == 1) {
            painter.drawRect(QRect(confetti.x(), confetti.y(), 5, 2));
            painter.drawRect(QRect(confetti.x() + 5, confetti.y() + 2, 5, 2));
        } else {
            painter.drawRect(QRect(confetti.x(), confetti.y(), 5, 2));
            painter.drawRect(QRect(confetti.x() + 5, confetti.y() - 2, 5, 2));
        }
        if (confetti.y() > 500) {
            confettis.remove(confettis.indexOf(confettiPair));
        }
    }

    // calls to redraw
    QTimer::singleShot(50, this, [this] { update(); });
}

void CelebrationScreen::addBalloon() {
    if (nextBalloonCount < 0) {
        int randomPosition = QRandomGenerator::global()->bounded(0, 800);
        balloons.push_back(QPair<QPoint, int>(QPoint(randomPosition, 500), 0));
        nextBalloonCount = 10;
    }
    nextBalloonCount--;
}

void CelebrationScreen::addExplosion() {
    if (nextExplosionCount < 0) {
        int randomX = QRandomGenerator::global()->bounded(50, 600);
        int randomY = QRandomGenerator::global()->bounded(50, 600);
        explosions.push_back(QPair<QPoint, int>(QPoint(randomX, randomY), 0));
        nextExplosionCount = 15;
    }
    nextExplosionCount--;
}

void CelebrationScreen::addConfetti() {
    for (int i = 0; i < 2; i++) {
        QColor color;
        int randomColor = QRandomGenerator::global()->bounded(0, 3);

        if (randomColor == 0) {
            color = Qt::red;
        } else if (randomColor == 1) {
            color = Qt::white;
        } else {
            color = Qt::blue;
        }

        int randomPosition = QRandomGenerator::global()->bounded(0, 700);
        confettis.push_back(
            QPair<QPoint, QColor>(QPoint(randomPosition, 0), color));
    }
}

CelebrationScreen::~CelebrationScreen() {
    delete ui;
}
