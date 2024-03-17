#include "simulation.h"
#include <QKeyEvent>
#include <QTimer>
#include <QGraphicsTextItem>
#include <QFont>
#include <QBrush>
#include <QImage>
#include "robot.h"
#include "controllablerobot.h"
#include <QPushButton>

QTimer * robotTimer;

Simulation::Simulation(QWidget *parent) {
    // create the scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,1000,800); // make the scene 800x600 instead of infinity by infinity (default)



    // make the newly created scene the scene to visualize (since Game is a QGraphicsView Widget,
    // it can be used to visualize scenes)
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(1000,800);

    // create a robot
    robot = new Robot(0,300,300, 150, 50, true, scene);
    Robot * robot2 = new Robot(0,100,300, 150, 50, true, scene);
    scene->addItem(robot);
    scene->addItem(robot2);

    //robot timer
    robotTimer = new QTimer(this);
    connect(robotTimer,SIGNAL(timeout()),robot,SLOT(move()));
    connect(robotTimer,SIGNAL(timeout()),robot2,SLOT(move()));
    robotTimer->start(5);

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
    QPushButton * createObstacleButton = new QPushButton("Create Obstacle", this);
    createObstacleButton->setGeometry(QRect(QPoint(10, 660), QSize(100, 50)));
    connect(createObstacleButton, SIGNAL(released()), this, SLOT(buildObstacle()));
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
            contRobot->rotate(5);
        }

    }
    // shoot with the spacebar
    else if (event->key() == Qt::Key_Left){
        if(robotTimer->isActive()){
            contRobot->rotate(-5);
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
    }
}

void Simulation::buildObstacle(){
    if(!build){
        build = new QGraphicsRectItem();
        build->setRect(0,0,50,50);
        setCursor(50);
    }
}

void Simulation::mousePressEvent(QMouseEvent * event){
    if(build){
        scene->addItem(build);
        build->setPos(event->pos());
        cursor = nullptr;
        build = nullptr;
    }
    else{
        QGraphicsView::mousePressEvent(event);
    }
}
