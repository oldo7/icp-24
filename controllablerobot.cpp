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
    //setFlag(QGraphicsItem::ItemIsFocusable);
    //setFocus();
}

void ControllableRobot::move(){
    // move enemy

    if(!checkNoObstacles()){
        return;
    }

    double dy = 3*sin(qDegreesToRadians(angle));
    double dx = 3*cos(qDegreesToRadians(angle));

    setPos(x() + dx,y() + dy);
    scanArea->setPos(x()+30,y());
}

void ControllableRobot::rotate(int angleToRotate){
    angle += angleToRotate;

    setTransformOriginPoint(15,15);
    scanArea->setTransformOriginPoint(-15, 15);
    setRotation(angle);
    scanArea->setRotation(angle);
}

void ControllableRobot::keyPressEvent(QKeyEvent *event){
    qDebug() << "i should move";
    // move the player left and right
    if (event->key() == Qt::Key_Up){
        if(robotTimer->isActive()){
            move();
        }

    }
    else if (event->key() == Qt::Key_Right){
        if(robotTimer->isActive()){
            rotate(5);
        }

    }
    // shoot with the spacebar
    else if (event->key() == Qt::Key_Left){
        if(robotTimer->isActive()){
            rotate(-5);
        }
    }
}
