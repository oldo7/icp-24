#include "simulation.h"
#include <QKeyEvent>
#include <QTimer>
#include <QGraphicsTextItem>
#include <QFont>
#include <QBrush>
#include <QImage>
#include "addrobot.h"
#include "qlabel.h"
#include "robot.h"
#include "controllablerobot.h"
#include "savestate.h"
#include <QPushButton>
#include <QLineEdit>
#include <QList>

AddRobot * addRobot;
QTimer * robotTimer;
saveState * saveCurrentState;
std::list<Robot*> robotList;
std::list<QGraphicsRectItem*> obstacleList;

Simulation::Simulation(QWidget *parent) {
    // create the scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,1000,850); // make the scene 800x600 instead of infinity by infinity (default)

    //robot timer
    robotTimer = new QTimer(this);
    //connect(robotTimer,SIGNAL(timeout()),robot2,SLOT(robotStep()));
    robotTimer->start(5);
    robotTimer->stop();         //Simulation starts paused


    // make the newly created scene the scene to visualize (since Game is a QGraphicsView Widget,
    // it can be used to visualize scenes)
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(1000,850);

    //zoznam robotov
    //std::list<Robot*> robotList;

    // create 2 starting robots
    robot = new Robot(0,300,300, 150, 50, true, 45, scene);
    Robot * robot2 = new Robot(0,100,300, 150, 45, true, 90, scene);
    scene->addItem(robot);
    scene->addItem(robot2);


    robotList.push_back(robot);
    robotList.push_back(robot2);

    //make a controllable robot todo: button
    //contRobot = new ControllableRobot(0,100,100,80,scene);
    //contRobot->setFlag(QGraphicsItem::ItemIsFocusable);
    //contRobot->setFocus();
    // add the player to the scene
    //scene->addItem(contRobot);

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
    pauseButton = new QPushButton("Start Simulation", this);
    pauseButton->setGeometry(QRect(QPoint(20, 785), QSize(200, 50)));
    pauseButton->setStyleSheet("font-size: 20px; font-weight: bold");
    connect(pauseButton, SIGNAL(released()), this, SLOT(pauseSimulation()));

    //set cursor
    cursor = nullptr;
    setMouseTracking(true);
    build = nullptr;

    //set obstacle size
    obstacleSize = new QLineEdit(this);
    obstacleSize->setGeometry(QRect(QPoint(130, 650), QSize(100, 50)));
    obstacleSize->setEchoMode(QLineEdit::Normal);
    obstacleSize->setPlaceholderText("Obstacle Size");
    obstacleSize->setInputMask("0000;_");
    obstacleSize->setText("60");

    //obstacle size label
    QLabel * obstacleSizeLabel = new QLabel(this);
    obstacleSizeLabel->setGeometry(QRect(QPoint(20, 650), QSize(100, 50)));
    obstacleSizeLabel->setText("Obstacle Size:");


    //new obstacle group label
    QLabel * obstacleGroup = new QLabel(this);
    obstacleGroup->setGeometry(QRect(QPoint(20, 600), QSize(150, 50)));
    obstacleGroup->setText("New Obstacle:");
    obstacleGroup->setStyleSheet("font-size: 18px; font-weight: bold; text-align: center");


    QGraphicsRectItem * obstacleGroupBorder = new QGraphicsRectItem(0);
    obstacleGroupBorder->setRect(15,610,225,150);
    scene->addItem(obstacleGroupBorder);



    //obstacle button
    QPushButton * createSmallObstacleButton = new QPushButton("Create Obstacle", this);
    createSmallObstacleButton->setGeometry(QRect(QPoint(20, 700), QSize(140, 50)));
    connect(createSmallObstacleButton, SIGNAL(released()), this, SLOT(buildSmallObstacle()));

    QPushButton * cancelBuildButton = new QPushButton("Cancel", this);
    cancelBuildButton->setGeometry(QRect(QPoint(160, 700), QSize(70, 50)));
    connect(cancelBuildButton, SIGNAL(released()), this, SLOT(cancelBuild()));

    //buttons for adding robots
    addRobot = new AddRobot(this);

    //saving and loading from/to file
    saveCurrentState = new saveState(this);
}


void Simulation::keyPressEvent(QKeyEvent *event){
    addRobot->keyPressEvent(event);
}


void Simulation::pauseSimulation(){
    if(robotTimer->isActive()){
        robotTimer->stop();
        pauseButton->setText("Resume Simulation");
    }
    else{
        robotTimer->start();
        pauseButton->setText("Pause Simulation");
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
    buildObstacle(obstacleSize->text().toInt());
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

            obstacleList.push_back(build);

            cursor = nullptr;
            build = nullptr;
            buildsize = 0;
        }
    }
    else{
        addRobot->mousePressEvent(event);
    }
}


