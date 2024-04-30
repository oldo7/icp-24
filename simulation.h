#ifndef SIMULATION_H
#define SIMULATION_H

#include <QGraphicsView>
#include <QWidget>
#include "qlineedit.h"
#include "robot.h"
#include <QGraphicsScene>
#include <QPushButton>

class Simulation: public QGraphicsView{
    Q_OBJECT
public:
    Simulation(QWidget * parent=0);
    void keyPressEvent(QKeyEvent *event);
    QGraphicsScene * scene;
    Robot * robot;
    QPushButton * pauseButton;
    QGraphicsRectItem * cursor;
    QGraphicsRectItem * build;
    int buildsize;
    void setCursor(int size);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void buildObstacle(int size);
    QLineEdit * obstacleSize;
public slots:
    void pauseSimulation();
    void buildSmallObstacle();
    void cancelBuild();
};

#endif // SIMULATION_H
