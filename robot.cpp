#include "robot.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QList>
#include "simulation.h"
#include <QDebug>
#include <QGraphicsPixmapItem>

extern Simulation * simulation;

Robot::Robot(QGraphicsItem *parent, int initX, int initY, int scanAreaSize, int rotateBy, bool rotateClockwise, QGraphicsScene * scene): QObject(), QGraphicsPixmapItem(parent){
    //set random x position
    setPixmap(QPixmap(":/images/robot.png"));
    //setRect(0,0,30,30);
    setPos(initX, initY);

    scanArea = new QGraphicsRectItem(this);
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
    //test
    rotate(45);
}

Robot::Robot(QGraphicsItem *parent, int initX, int initY, int scanAreaSize, QGraphicsScene * scene){
    setPixmap(QPixmap(":/images/robot.png"));       //todo: zmenit
    //setRect(0,0,30,30);
    setPos(initX, initY);

    scanArea = new QGraphicsRectItem(this);
    scanArea->setRect(0,0,scanAreaSize,30);
    scanArea->setPos(x()+30,y());
    scene->addItem(scanArea);

    //TODO: change to white, 0
    QPen pen(Qt::black, 1);
    scanArea->setPen(pen);
}

void Robot::move(){
    // move enemy down

    if(!checkNoObstacles()){
        rotate(rotateAngle);
        return;
    }

    double dy = 1*sin(qDegreesToRadians(angle));
    double dx = 1*cos(qDegreesToRadians(angle));

    setPos(x() + dx,y() + dy);
    scanArea->setPos(x()+30,y());


}

void Robot::rotate(double angleToRotate){
    angle += angleToRotate;
    setTransformOriginPoint(15,15);
    scanArea->setTransformOriginPoint(-15, 15);
    setRotation(angle);
    scanArea->setRotation(angle);
}

bool Robot::checkNoObstacles(){
    QList<QGraphicsItem *> colliding_items = scanArea->collidingItems();
    qDebug() << colliding_items;
    return colliding_items.size() == 1;
}
