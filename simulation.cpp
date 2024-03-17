#include "simulation.h"

#include <QTimer>
#include <QGraphicsTextItem>
#include <QFont>
#include <QBrush>
#include <QImage>
#include "robot.h"
#include "controllablerobot.h"

Simulation::Simulation(QWidget *parent) {
    // create the scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,800,600); // make the scene 800x600 instead of infinity by infinity (default)

    // make the newly created scene the scene to visualize (since Game is a QGraphicsView Widget,
    // it can be used to visualize scenes)
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(800,600);

    // create a robot
    robot = new Robot(0,300,300, 150, 50, true, scene);
    scene->addItem(robot);

    //make a controllable robot
    ControllableRobot * contRobot = new ControllableRobot(0,100,100,80,scene);
    contRobot->setFlag(QGraphicsItem::ItemIsFocusable);
    contRobot->setFocus();
    // add the player to the scene
    scene->addItem(contRobot);

    QGraphicsRectItem * boxq;
    boxq = new QGraphicsRectItem(0);
    boxq->setPos(100,500);
    boxq->setRect(0,0,600,100);
    //robot->setFlag(QGraphicsItem::ItemIsFocusable);
    //robot->setFocus();
    scene->addItem(boxq);


    /*
    // create the score/health
    score = new Score();
    scene->addItem(score);
    health = new Health();
    health->setPos(health->x(),health->y()+25);
    scene->addItem(health);
    */

    /*
    // spawn enemies
    QTimer * timer = new QTimer();
    QObject::connect(timer,SIGNAL(timeout()),player,SLOT(spawn()));
    timer->start(2000);
    */
}
