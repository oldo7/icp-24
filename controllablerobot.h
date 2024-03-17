#ifndef CONTROLLABLEROBOT_H
#define CONTROLLABLEROBOT_H

#include "robot.h"
class ControllableRobot : public Robot
{
public:
    ControllableRobot(QGraphicsItem *parent, int initX, int initY, int scanAreaSize, QGraphicsScene * scene);
    void move();
};

#endif // CONTROLLABLEROBOT_H
