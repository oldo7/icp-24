#include "addrobot.h"
#include "qlineedit.h"
#include <QLineEdit>
#include "simulation.h"
#include <QPushButton>

#include "simulation.h"
#include <QKeyEvent>
#include <QTimer>
#include <QGraphicsTextItem>
#include <QFont>
#include <QBrush>
#include <QImage>
#include "addrobot.h"
#include <QPushButton>
#include <QLineEdit>
#include <QDebug>

#include <iostream>
#include <stdlib.h>

AddRobot::AddRobot(Simulation * simulation) : QObject() {
    simulationScene = simulation;
    rotation = new QLineEdit(simulation);
    rotation->setGeometry(QRect(QPoint(230, 610), QSize(100, 50)));
    rotation->setEchoMode(QLineEdit::Normal);
    rotation->setPlaceholderText("Initial Rotation");
    rotation->setInputMask("000;_");

    detectionArea = new QLineEdit(simulation);
    detectionArea->setGeometry(QRect(QPoint(230, 660), QSize(100, 50)));
    detectionArea->setEchoMode(QLineEdit::Normal);
    detectionArea->setPlaceholderText("Detection Area");
    detectionArea->setInputMask("000;_");

    turnAngle = new QLineEdit(simulation);
    turnAngle->setGeometry(QRect(QPoint(230, 710), QSize(100, 50)));
    turnAngle->setEchoMode(QLineEdit::Normal);
    turnAngle->setPlaceholderText("Turn angle");
    turnAngle->setInputMask("000;_");

    QPushButton * newRobotButton = new QPushButton("New Robot", simulation);
    newRobotButton->setGeometry(QRect(QPoint(120, 710), QSize(100, 50)));
    connect(newRobotButton, SIGNAL(released()), this, SLOT(newRobot()));

    robotCursor = nullptr;
    robotBuild = false;
}

void AddRobot::newRobot(){
    qDebug() << rotation->text() << turnAngle->text() << detectionArea->text() << "build mode initiated";
    setRobotCursor();
    robotBuild = true;
}

//cursor
void AddRobot::setRobotCursor(){
    if(!simulationScene->cursor){
        if(robotCursor){
            simulationScene->scene->removeItem(robotCursor);
            delete robotCursor;
        }
        robotCursor = new QGraphicsPixmapItem(0);
        robotCursor->setPixmap(QPixmap(":/images/robot.png"));
        simulationScene->scene->addItem(robotCursor);
    }
}

void AddRobot::mouseMoveEvent(QMouseEvent * event){
    if(robotCursor){
        robotCursor->setPos(event->pos());
    }
}

/*
void Simulation::cancelBuild(){
    if(build){
        scene->removeItem(cursor);
        delete build;
        delete cursor;
        cursor = nullptr;
        build = nullptr;
        buildsize = 0;
    }
}
*/

void AddRobot::mousePressEvent(QMouseEvent * event){
    qDebug() << "clicked in addrobot" << robotBuild;
    if(robotBuild){
        if(event->pos().x() + 30 < 1000 && event->pos().y() + 30 < 600){
            qDebug() << "creating new robot";
            Robot * newRobot = new Robot(0, event->pos().x(), event->pos().y(),detectionArea->text().toInt(), turnAngle->text().toInt(), true,rotation->text().toInt(),simulationScene->scene);
            simulationScene->scene->addItem(newRobot);
            simulationScene->scene->removeItem(robotCursor);
            delete robotCursor;
            robotCursor = nullptr;
            robotBuild = false;
        }
    }
    //else{
    //    QGraphicsView::mousePressEvent(event);
    //}
}
