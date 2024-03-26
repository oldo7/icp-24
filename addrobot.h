#ifndef ADDROBOT_H
#define ADDROBOT_H

#include "controllablerobot.h"
#include "qlineedit.h"
#include "simulation.h"
#include <QGraphicsPixmapItem>

class AddRobot : QObject
{
    Q_OBJECT
private:
    Simulation * simulationScene;
    QLineEdit * turnAngle;
    QLineEdit * rotation;
    QLineEdit * detectionArea;
    QLineEdit * controllableDetectionArea;
    void setRobotCursor();
    int robotBuild;
    ControllableRobot * contRobot;
    QPushButton * turnDirection;
    bool turnClockwise;
public:
    AddRobot(Simulation * simulation);
    QGraphicsPixmapItem * robotCursor;
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
public slots:
    void newRobot();
    void newControllableRobot();
    void setDirection();
    void turnLeftPressed();
    void turnRightPressed();
    void turnUpPressed();
    void cancelRobot();
};

#endif // ADDROBOT_H
