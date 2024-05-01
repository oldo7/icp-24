#include "robot.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QList>
#include "scanareat.h"
#include "simulation.h"
#include <QDebug>
#include <QGraphicsPixmapItem>

#include "unistd.h"
#include <QThread>

extern Simulation * simulation;
extern QTimer * robotTimer;

Robot::Robot(QGraphicsItem *parent, int initX, int initY, int scanAreaSize, int rotateBy, bool rotateClockwise, int initRotate, QGraphicsScene * scene): QObject(), QGraphicsPixmapItem(parent){
    //subscribe to the global robot timer
    connect(robotTimer,SIGNAL(timeout()),this,SLOT(robotStep()));

    setPixmap(QPixmap(":/images/robot.png"));
    //setRect(0,0,30,30);
    setPos(initX, initY);

    scanSize = scanAreaSize;
    scanArea = new scanAreaT();
    scanArea->setRect(0,0,scanAreaSize,30);
    scanArea->setPos(x()+30,y());
    scene->addItem(scanArea);

    //TODO: change to white, 0
    QPen pen(Qt::black, 1);
    scanArea->setPen(pen);

    rotateAngle = rotateBy;

    if(!rotateClockwise){
        rotateAngle *= -1;
    }

    angle += initRotate;
    setTransformOriginPoint(15,15);
    scanArea->setTransformOriginPoint(-15, 15);
    setRotation(angle);
    scanArea->setRotation(angle);
}

Robot::Robot(QGraphicsItem *parent, int initX, int initY, int scanAreaSize, QGraphicsScene * scene){
    setPixmap(QPixmap(":/images/controllable.png"));       //todo: zmenit: controllable robot
    //setRect(0,0,30,30);
    setPos(initX, initY);

    scanSize = scanAreaSize;
    scanArea = new scanAreaT();
    scanArea->setRect(0,0,scanAreaSize,30);
    scanArea->setPos(x()+30,y());
    scene->addItem(scanArea);

    //TODO: change to white, 0
    QPen pen(Qt::black, 1);
    scanArea->setPen(pen);
}

void Robot::robotStep(){
    if(angleToRotate == 0){
        move();
    }else{
        rotate();
    }
}

void Robot::move(){
    if(!checkNoObstacles()){
        setRotate(rotateAngle);
        return;
    }

    double dy = 1*sin(qDegreesToRadians(angle));
    double dx = 1*cos(qDegreesToRadians(angle));

    setPos(x() + dx,y() + dy);
    scanArea->setPos(x()+30,y());


}

void Robot::rotate(){
    if(angleToRotate > 0){
        angle += 1;
        angleToRotate -= 1;
    }
    else{
        angle -= 1;
        angleToRotate += 1;
    }

    setTransformOriginPoint(15,15);
    scanArea->setTransformOriginPoint(-15, 15);
    setRotation(angle);
    scanArea->setRotation(angle);
}

void Robot::setRotate(int angle){
    angleToRotate += angle;
}

bool Robot::checkNoObstacles(){
    QList<QGraphicsItem *> colliding_items = scanArea->collidingItems();
    int collisions = colliding_items.size();
    for( size_t i = 0, n = colliding_items.size(); i < n; i++){
        scanAreaT * scan = dynamic_cast<scanAreaT *>(colliding_items[i]);
        if(scan){
            collisions -= 1;
        }
    }
    return collisions == 1;
}
