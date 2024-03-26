#include "controllablerobot.h"
#include "qtimer.h"
#include "robot.h"
#include <QGraphicsItem>
#include <QList>
#include "simulation.h"
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QKeyEvent>

extern QTimer * robotTimer;

ControllableRobot::ControllableRobot(QGraphicsItem *parent, int initX, int initY, int scanAreaSize, QGraphicsScene * scene) : Robot(parent, initX, initY, scanAreaSize, scene){

}

void ControllableRobot::move(){
    // move enemy

    if(!checkNoObstacles()){
        return;
    }

    double dy = 1*sin(qDegreesToRadians(angle));
    double dx = 1*cos(qDegreesToRadians(angle));

    setPos(x() + dx,y() + dy);
    scanArea->setPos(x()+30,y());
}
