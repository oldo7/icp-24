#include "addrobot.h"
#include "controllablerobot.h"
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
#include <QLabel>

#include <iostream>
#include <stdlib.h>

extern QTimer * robotTimer;

AddRobot::AddRobot(Simulation * simulation) : QObject() {
    contRobot = nullptr;
    simulationScene = simulation;

    QLabel * rotationLabel = new QLabel(simulation);
    rotationLabel->setGeometry(QRect(QPoint(300, 710), QSize(100, 50)));
    rotationLabel->setText("Initial Rotation: ");

    QLabel * detectionLabel = new QLabel(simulation);
    detectionLabel->setGeometry(QRect(QPoint(300, 660), QSize(100, 50)));
    detectionLabel->setText("Detection Area: ");

    QLabel * angleLabel = new QLabel(simulation);
    angleLabel->setGeometry(QRect(QPoint(300, 610), QSize(100, 50)));
    angleLabel->setText("Turning angle: ");


    rotation = new QLineEdit(simulation);
    rotation->setGeometry(QRect(QPoint(400, 710), QSize(100, 50)));
    rotation->setEchoMode(QLineEdit::Normal);
    rotation->setPlaceholderText("Initial Rotation");
    rotation->setInputMask("000;_");
    rotation->setText("0");

    detectionArea = new QLineEdit(simulation);
    detectionArea->setGeometry(QRect(QPoint(400, 660), QSize(100, 50)));
    detectionArea->setEchoMode(QLineEdit::Normal);
    detectionArea->setPlaceholderText("Detection Area");
    detectionArea->setInputMask("000;_");
    detectionArea->setText("60");

    turnAngle = new QLineEdit(simulation);
    turnAngle->setGeometry(QRect(QPoint(400, 610), QSize(100, 50)));
    turnAngle->setEchoMode(QLineEdit::Normal);
    turnAngle->setPlaceholderText("Turn angle");
    turnAngle->setInputMask("000;_");
    turnAngle->setText("30");

    turnClockwise = true;
    turnDirection = new QPushButton("Clockwise", simulation);
    turnDirection->setGeometry(QRect(QPoint(500, 610), QSize(100, 50)));
    connect(turnDirection, SIGNAL(released()), this, SLOT(setDirection()));

    QPushButton * newRobotButton = new QPushButton("New Robot", simulation);
    newRobotButton->setGeometry(QRect(QPoint(500, 660), QSize(100, 50)));
    connect(newRobotButton, SIGNAL(released()), this, SLOT(newRobot()));

    controllableDetectionArea = new QLineEdit(simulation);
    controllableDetectionArea->setGeometry(QRect(QPoint(650, 610), QSize(100, 50)));
    controllableDetectionArea->setEchoMode(QLineEdit::Normal);
    controllableDetectionArea->setPlaceholderText("Controllable Robot Area");
    controllableDetectionArea->setInputMask("000;_");
    controllableDetectionArea->setText("80");

    QPushButton * newControllableRobotButton = new QPushButton("New Controllable Robot", simulation);
    newControllableRobotButton->setGeometry(QRect(QPoint(650, 660), QSize(100, 50)));
    connect(newControllableRobotButton, SIGNAL(released()), this, SLOT(newControllableRobot()));

    //robot controls
    QPushButton * turnLeftButton = new QPushButton("<", simulation);
    turnLeftButton->setGeometry(QRect(QPoint(750, 610), QSize(50, 50)));
    turnLeftButton->setAutoRepeat(true);
    turnLeftButton->setAutoRepeatInterval(40);
    connect(turnLeftButton, SIGNAL(pressed()), this, SLOT(turnLeftPressed()));

    QPushButton * turnRightButton = new QPushButton(">", simulation);
    turnRightButton->setGeometry(QRect(QPoint(850, 610), QSize(50, 50)));
    turnRightButton->setAutoRepeat(true);
    turnRightButton->setAutoRepeatInterval(40);
    connect(turnRightButton, SIGNAL(pressed()), this, SLOT(turnRightPressed()));

    QPushButton * turnUpButton = new QPushButton("↑", simulation);
    turnUpButton->setGeometry(QRect(QPoint(800, 610), QSize(50, 50)));
    turnUpButton->setAutoRepeat(true);
    turnUpButton->setAutoRepeatInterval(20);
    connect(turnUpButton, SIGNAL(pressed()), this, SLOT(turnUpPressed()));

    QPushButton * cancelRobotButton = new QPushButton("Cancel", simulation);
    cancelRobotButton->setGeometry(QRect(QPoint(500, 710), QSize(100, 50)));
    connect(cancelRobotButton, SIGNAL(released()), this, SLOT(cancelRobot()));

    robotCursor = nullptr;
    robotBuild = 0;
}

void AddRobot::setDirection(){
    if(turnClockwise){
        turnClockwise = false;
        turnDirection->setText("Counterclockwise");
    }else{
        turnClockwise = true;
        turnDirection->setText("Clockwise");
    }
}

void AddRobot::newRobot(){
    qDebug() << rotation->text() << turnAngle->text() << detectionArea->text() << "build mode initiated";
    robotBuild = 1;
    setRobotCursor();
}

void AddRobot::newControllableRobot(){
    if(contRobot == nullptr){
        qDebug() << rotation->text() << turnAngle->text() << detectionArea->text() << "build mode initiated";
        robotBuild = 2;
        setRobotCursor();
    }
}

//cursor
void AddRobot::setRobotCursor(){
    if(!simulationScene->cursor){
        if(robotCursor){
            simulationScene->scene->removeItem(robotCursor);
            delete robotCursor;
        }
        robotCursor = new QGraphicsPixmapItem(0);
        qDebug() << robotBuild;
        if(robotBuild == 1){
            robotCursor->setPixmap(QPixmap(":/images/robot.png"));
        }
        else{
            robotCursor->setPixmap(QPixmap(":/images/controllable.png"));
        }

        simulationScene->scene->addItem(robotCursor);
    }
}

void AddRobot::mouseMoveEvent(QMouseEvent * event){
    if(robotCursor){
        robotCursor->setPos(event->pos());
    }
}

void AddRobot::keyPressEvent(QKeyEvent *event){
    if(contRobot){
        contRobot->keyPressEvent(event);
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
    if(robotBuild == 1){
        if(event->pos().x() + 30 < 1000 && event->pos().y() + 30 < 600){
            qDebug() << "creating new robot";
            Robot * newRobot = new Robot(0, event->pos().x(), event->pos().y(),detectionArea->text().toInt(), turnAngle->text().toInt(), turnClockwise,rotation->text().toInt(),simulationScene->scene);
            simulationScene->scene->addItem(newRobot);
            simulationScene->scene->removeItem(robotCursor);
            delete robotCursor;
            robotCursor = nullptr;
            robotBuild = 0;
        }
    }
    if(robotBuild == 2){
        if(event->pos().x() + 30 < 1000 && event->pos().y() + 30 < 600){
            qDebug() << "creating new controllable robot";
            contRobot = new ControllableRobot(0, event->pos().x(), event->pos().y(),controllableDetectionArea->text().toInt(), simulationScene->scene);
            simulationScene->scene->addItem(contRobot);
            simulationScene->scene->removeItem(robotCursor);
            delete robotCursor;
            robotCursor = nullptr;
            robotBuild = 0;
        }
    }
    //else{
    //    QGraphicsView::mousePressEvent(event);
    //}
}

void AddRobot::turnLeftPressed(){
    if(robotTimer->isActive() && contRobot){
        contRobot->rotate(-5);
    }
}

void AddRobot::turnRightPressed(){
    if(robotTimer->isActive() && contRobot){
        contRobot->rotate(5);
    }
}

void AddRobot::turnUpPressed(){
    if(robotTimer->isActive() && contRobot){
        contRobot->move();
    }
}

void AddRobot::cancelRobot(){
    if(robotBuild != 0){
        simulationScene->scene->removeItem(robotCursor);
        delete robotCursor;
        robotCursor = nullptr;
        robotBuild = 0;
    }
}
