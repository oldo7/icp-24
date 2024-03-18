#include "simulation.h"
#include <QKeyEvent>
#include <QTimer>
#include <QGraphicsTextItem>
#include <QFont>
#include <QBrush>
#include <QImage>
#include "addrobot.h"
#include "robot.h"
#include "controllablerobot.h"
#include <QPushButton>
#include <QLineEdit>

AddRobot * addRobot;
QTimer * robotTimer;

Simulation::Simulation(QWidget *parent) {
    // create the scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,1000,800); // make the scene 800x600 instead of infinity by infinity (default)

    //robot timer
    robotTimer = new QTimer(this);
    //connect(robotTimer,SIGNAL(timeout()),robot2,SLOT(robotStep()));
    robotTimer->start(5);


    // make the newly created scene the scene to visualize (since Game is a QGraphicsView Widget,
    // it can be used to visualize scenes)
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(1000,800);

    // create a robot
    robot = new Robot(0,300,300, 150, 50, true, 45, scene);
    Robot * robot2 = new Robot(0,100,300, 150, 45, true, 90, scene);
    scene->addItem(robot);
    scene->addItem(robot2);


    //make a controllable robot
    contRobot = new ControllableRobot(0,100,100,80,scene);
    contRobot->setFlag(QGraphicsItem::ItemIsFocusable);
    contRobot->setFocus();
    // add the player to the scene
    scene->addItem(contRobot);

    QGraphicsLineItem * right = new QGraphicsLineItem(0);
    right->setLine(1000,0,1000,800);
    scene->addItem(right);
    QGraphicsLineItem * left = new QGraphicsLineItem(0);
    left->setLine(0,0,0,800);
    scene->addItem(left);
    QGraphicsLineItem * top = new QGraphicsLineItem(0);
    top->setLine(0,0,1000,0);
    scene->addItem(top);
    QGraphicsLineItem * down = new QGraphicsLineItem(0);
    down->setLine(0,600,1000,600);
    scene->addItem(down);

    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 12, Qt::DashDotLine, Qt::RoundCap));
    painter.drawLine(5, 5, 0, 300);


    //pause / resume button
    // Create the button, make "this" the parent
    pauseButton = new QPushButton("Pause / Resume", this);
    // set size and location of the button
    pauseButton->setGeometry(QRect(QPoint(10, 610), QSize(100, 50)));

    //set cursor
    cursor = nullptr;
    setMouseTracking(true);
    build = nullptr;

    // set size and location of the button
    pauseButton->setGeometry(QRect(QPoint(10, 610), QSize(100, 50)));

    // Connect button signal to appropriate slot
    connect(pauseButton, SIGNAL(released()), this, SLOT(pauseSimulation()));

    //obstacle button
    QPushButton * createSmallObstacleButton = new QPushButton("Create small Obstacle", this);
    createSmallObstacleButton->setGeometry(QRect(QPoint(10, 660), QSize(100, 50)));
    connect(createSmallObstacleButton, SIGNAL(released()), this, SLOT(buildSmallObstacle()));

    QPushButton * createMediumObstacleButton = new QPushButton("Create medium Obstacle", this);
    createMediumObstacleButton->setGeometry(QRect(QPoint(10, 710), QSize(100, 50)));
    connect(createMediumObstacleButton, SIGNAL(released()), this, SLOT(buildMediumObstacle()));

    QPushButton * createLargeObstacleButton = new QPushButton("Create large Obstacle", this);
    createLargeObstacleButton->setGeometry(QRect(QPoint(120, 610), QSize(100, 50)));
    connect(createLargeObstacleButton, SIGNAL(released()), this, SLOT(buildLargeObstacle()));

    QPushButton * cancelBuildButton = new QPushButton("Cancel", this);
    cancelBuildButton->setGeometry(QRect(QPoint(120, 660), QSize(100, 50)));
    connect(cancelBuildButton, SIGNAL(released()), this, SLOT(cancelBuild()));

    addRobot = new AddRobot(this);
}

void Simulation::keyPressEvent(QKeyEvent *event){
    // move the player left and right
    if (event->key() == Qt::Key_Up){
        if(robotTimer->isActive()){
            contRobot->move();
        }

    }
    else if (event->key() == Qt::Key_Right){
        if(robotTimer->isActive()){
            contRobot->setRotate(5);
        }

    }
    // shoot with the spacebar
    else if (event->key() == Qt::Key_Left){
        if(robotTimer->isActive()){
            contRobot->setRotate(-5);
        }
    }
    else if (event->key() == Qt::Key_Down){
        pauseSimulation();
    }
}

void Simulation::pauseSimulation(){
    if(robotTimer->isActive()){
        robotTimer->stop();
    }
    else{
        robotTimer->start();
    }
}

void Simulation::setCursor(int size){
    if(cursor){
        scene->removeItem(cursor);
        delete cursor;
    }
    cursor = new QGraphicsRectItem(0);
    cursor->setRect(0,0,size,size);
    scene->addItem(cursor);
}

void Simulation::mouseMoveEvent(QMouseEvent * event){
    if(cursor){
        cursor->setPos(event->pos());
    }else{
        addRobot->mouseMoveEvent(event);
    }
}

void Simulation::buildSmallObstacle(){
    buildObstacle(50);
}

void Simulation::buildMediumObstacle(){
    buildObstacle(100);
}

void Simulation::buildLargeObstacle(){
    buildObstacle(200);
}

void Simulation::buildObstacle(int size){
    build = new QGraphicsRectItem();
    build->setRect(0,0,size,size);
    setCursor(size);
    buildsize = size;
}

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

void Simulation::mousePressEvent(QMouseEvent * event){
    if(build){
        if(event->pos().x() + buildsize < 1000 && event->pos().y() + buildsize < 600){
            qDebug() << event->pos().x();
            scene->addItem(build);
            build->setPos(event->pos());
            cursor = nullptr;
            build = nullptr;
            buildsize = 0;
        }
    }
    else{
        addRobot->mousePressEvent(event);
    }
}


