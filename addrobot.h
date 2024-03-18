#ifndef ADDROBOT_H
#define ADDROBOT_H

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
    void setRobotCursor();
    bool robotBuild;
public:
    AddRobot(Simulation * simulation);
    QGraphicsPixmapItem * robotCursor;
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent *event);
public slots:
    void newRobot();
};

#endif // ADDROBOT_H
