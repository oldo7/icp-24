#ifndef SIMULATION_H
#define SIMULATION_H

#include <QGraphicsView>
#include <QWidget>
#include "controllablerobot.h"
#include "robot.h"
#include <QGraphicsScene>

class Simulation: public QGraphicsView{
public:
    Simulation(QWidget * parent=0);
    void keyPressEvent(QKeyEvent *event);
    ControllableRobot * contRobot;
    QGraphicsScene * scene;
    Robot * robot;
};

#endif // SIMULATION_H
