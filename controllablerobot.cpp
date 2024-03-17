#include "controllablerobot.h"
#include "robot.h"
#include <QGraphicsItem>
#include <QList>
#include "simulation.h"
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QKeyEvent>

ControllableRobot::ControllableRobot(QGraphicsItem *parent, int initX, int initY, int scanAreaSize, QGraphicsScene * scene) : Robot(parent, initX, initY, scanAreaSize, scene){

}

void ControllableRobot::keyPressEvent(QKeyEvent *event){
    // move the player left and right
    if (event->key() == Qt::Key_Up){
        move();
    }
    else if (event->key() == Qt::Key_Right){
        rotate(5);
    }
    // shoot with the spacebar
    else if (event->key() == Qt::Key_Left){
        rotate(-5);
    }
}

void ControllableRobot::move(){
    // move enemy down

    if(!checkNoObstacles()){
        return;
    }

    double dy = 1*sin(qDegreesToRadians(angle));
    double dx = 1*cos(qDegreesToRadians(angle));

    setPos(x() + dx,y() + dy);
    scanArea->setPos(x()+30,y());
}
