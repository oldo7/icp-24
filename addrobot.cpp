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
extern std::list<Robot*> robotList;

AddRobot::AddRobot(Simulation * simulation) : QObject() {
    contRobot = nullptr;
    simulationScene = simulation;

    QLabel * rotationLabel = new QLabel(simulation);
    rotationLabel->setGeometry(QRect(QPoint(300, 740), QSize(100, 50)));
    rotationLabel->setText("Initial Rotation: ");

    QLabel * detectionLabel = new QLabel(simulation);
    detectionLabel->setGeometry(QRect(QPoint(300, 690), QSize(100, 50)));
    detectionLabel->setText("Detection Area: ");

    QLabel * angleLabel = new QLabel(simulation);
    angleLabel->setGeometry(QRect(QPoint(300, 640), QSize(100, 50)));
    angleLabel->setText("Turning angle: ");

    //autonomous robot group label
    QLabel * addRobotGroupLabel = new QLabel(simulation);
    addRobotGroupLabel->setGeometry(QRect(QPoint(300, 600), QSize(250, 40)));
    addRobotGroupLabel->setStyleSheet("font-size: 18px; font-weight: bold; text-align: center");
    addRobotGroupLabel->setText("New autonomous robot:");

    QGraphicsLineItem * autonomousGroupTop = new QGraphicsLineItem(0);
    autonomousGroupTop->setLine(290,605,610,605);
    simulationScene->scene->addItem(autonomousGroupTop);

    QGraphicsLineItem * autonomousGroupLeft = new QGraphicsLineItem(0);
    autonomousGroupLeft->setLine(290,605,290,845);
    simulationScene->scene->addItem(autonomousGroupLeft);

    QGraphicsLineItem * autonomousGroupRight = new QGraphicsLineItem(0);
    autonomousGroupRight->setLine(610,605,610,750);
    simulationScene->scene->addItem(autonomousGroupRight);

    QGraphicsLineItem * autonomousGroupRight2 = new QGraphicsLineItem(0);
    autonomousGroupRight2->setLine(610,750,510,750);
    simulationScene->scene->addItem(autonomousGroupRight2);

    QGraphicsLineItem * autonomousGroupRight3 = new QGraphicsLineItem(0);
    autonomousGroupRight3->setLine(510,750,510,845);
    simulationScene->scene->addItem(autonomousGroupRight3);

    QGraphicsLineItem * autonomousGroupBottom = new QGraphicsLineItem(0);
    autonomousGroupBottom->setLine(510,845,290,845);
    simulationScene->scene->addItem(autonomousGroupBottom);


    rotation = new QLineEdit(simulation);
    rotation->setGeometry(QRect(QPoint(400, 740), QSize(100, 50)));
    rotation->setEchoMode(QLineEdit::Normal);
    rotation->setPlaceholderText("Initial Rotation");
    rotation->setInputMask("000;_");
    rotation->setText("0");

    detectionArea = new QLineEdit(simulation);
    detectionArea->setGeometry(QRect(QPoint(400, 690), QSize(200, 50)));
    detectionArea->setEchoMode(QLineEdit::Normal);
    detectionArea->setPlaceholderText("Detection Area");
    detectionArea->setInputMask("000;_");
    detectionArea->setText("60");

    turnAngle = new QLineEdit(simulation);
    turnAngle->setGeometry(QRect(QPoint(400, 640), QSize(200, 50)));
    turnAngle->setEchoMode(QLineEdit::Normal);
    turnAngle->setPlaceholderText("Turn angle");
    turnAngle->setInputMask("000;_");
    turnAngle->setText("30");

    turnClockwise = true;
    turnDirection = new QPushButton("Clockwise", simulation);
    turnDirection->setGeometry(QRect(QPoint(500, 640), QSize(100, 50)));
    connect(turnDirection, SIGNAL(released()), this, SLOT(setDirection()));

    QPushButton * newRobotButton = new QPushButton("Create", simulation);
    newRobotButton->setGeometry(QRect(QPoint(400, 790), QSize(100, 50)));
    connect(newRobotButton, SIGNAL(released()), this, SLOT(newRobot()));

    //controllable robot
    QLabel * controllableGroupLabel = new QLabel(simulation);
    controllableGroupLabel->setGeometry(QRect(QPoint(650, 600), QSize(250, 40)));
    controllableGroupLabel->setStyleSheet("font-size: 18px; font-weight: bold; text-align: center");
    controllableGroupLabel->setText("Controllable robot:");

    QGraphicsLineItem * controllableGroupLeft = new QGraphicsLineItem(0);
    controllableGroupLeft->setLine(645,605,645,750);
    simulationScene->scene->addItem(controllableGroupLeft);

    QGraphicsLineItem * controllableGroupLeft2 = new QGraphicsLineItem(0);
    controllableGroupLeft2->setLine(645,750,695,750);
    simulationScene->scene->addItem(controllableGroupLeft2);

    QGraphicsLineItem * controllableGroupLeft3 = new QGraphicsLineItem(0);
    controllableGroupLeft3->setLine(695,750,695,815);
    simulationScene->scene->addItem(controllableGroupLeft3);

    QGraphicsLineItem * controllableGroupBottom = new QGraphicsLineItem(0);
    controllableGroupBottom->setLine(695,815,860,815);
    simulationScene->scene->addItem(controllableGroupBottom);

    QGraphicsLineItem * controllableGroupRight = new QGraphicsLineItem(0);
    controllableGroupRight->setLine(860,815,860,605);
    simulationScene->scene->addItem(controllableGroupRight);

    QGraphicsLineItem * controllableGroupTop = new QGraphicsLineItem(0);
    controllableGroupTop->setLine(860,605,645,605);
    simulationScene->scene->addItem(controllableGroupTop);

    controllableDetectionArea = new QLineEdit(simulation);
    controllableDetectionArea->setGeometry(QRect(QPoint(750, 700), QSize(95, 50)));
    controllableDetectionArea->setEchoMode(QLineEdit::Normal);
    controllableDetectionArea->setPlaceholderText("Controllable Robot Area");
    controllableDetectionArea->setInputMask("000;_");
    controllableDetectionArea->setText("80");

    QPushButton * newControllableRobotButton = new QPushButton("New Controllable Robot", simulation);
    newControllableRobotButton->setGeometry(QRect(QPoint(710, 755), QSize(140, 50)));
    connect(newControllableRobotButton, SIGNAL(released()), this, SLOT(newControllableRobot()));

    QLabel * controllableAreaLabel = new QLabel(simulation);
    controllableAreaLabel->setGeometry(QRect(QPoint(650, 700), QSize(100, 50)));
    controllableAreaLabel->setText("Detection Area: ");

    //robot controls
    QPushButton * turnLeftButton = new QPushButton("<", simulation);
    turnLeftButton->setGeometry(QRect(QPoint(650, 635), QSize(65, 60)));
    turnLeftButton->setAutoRepeat(true);
    turnLeftButton->setAutoRepeatInterval(40);
    connect(turnLeftButton, SIGNAL(pressed()), this, SLOT(turnLeftPressed()));

    QPushButton * turnRightButton = new QPushButton(">", simulation);
    turnRightButton->setGeometry(QRect(QPoint(780, 635), QSize(65, 60)));
    turnRightButton->setAutoRepeat(true);
    turnRightButton->setAutoRepeatInterval(40);
    connect(turnRightButton, SIGNAL(pressed()), this, SLOT(turnRightPressed()));

    QPushButton * turnUpButton = new QPushButton("↑", simulation);
    turnUpButton->setGeometry(QRect(QPoint(715, 635), QSize(65, 60)));
    turnUpButton->setAutoRepeat(true);
    turnUpButton->setAutoRepeatInterval(20);
    connect(turnUpButton, SIGNAL(pressed()), this, SLOT(turnUpPressed()));

    QPushButton * cancelRobotButton = new QPushButton("Cancel Build", simulation);
    cancelRobotButton->setGeometry(QRect(QPoint(520, 760), QSize(160, 60)));
    cancelRobotButton->setStyleSheet("font-size: 18px");
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
    if(robotBuild == 1){
        if(event->pos().x() + 30 < 1000 && event->pos().y() + 30 < 600){
            Robot * newRobot = new Robot(0, event->pos().x(), event->pos().y(),detectionArea->text().toInt(), turnAngle->text().toInt(), turnClockwise,rotation->text().toInt(),simulationScene->scene);
            simulationScene->scene->addItem(newRobot);
            simulationScene->scene->removeItem(robotCursor);
            delete robotCursor;
            robotCursor = nullptr;
            robotBuild = 0;
            robotList.push_back(newRobot);

        }
    }
    if(robotBuild == 2){
        if(event->pos().x() + 30 < 1000 && event->pos().y() + 30 < 600){
            contRobot = new ControllableRobot(0, event->pos().x(), event->pos().y(),controllableDetectionArea->text().toInt(), simulationScene->scene);
            simulationScene->scene->addItem(contRobot);
            simulationScene->scene->removeItem(robotCursor);
            delete robotCursor;
            robotCursor = nullptr;
            robotBuild = 0;
            //pridat do zoznamu
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
