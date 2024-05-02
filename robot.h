#ifndef ROBOT_H
#define ROBOT_H
#include "scanareat.h"
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>
#include <cmath>
#include <QtMath>

class Robot:public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
protected:
    bool checkNoObstacles();
    void move();
    void rotate();
    scanAreaT * scanArea;
public:
    double angle;
    int scanSize;
    int rotateAngle;
    double angleToRotate;
    Robot(QGraphicsItem *parent, int initX, int initY, int scanAreaSize, int rotateAngle, bool rotateClockwise, int initRotate, QGraphicsScene * scene);
    Robot(QGraphicsItem *parent, int initX, int initY, int scanAreaSize, QGraphicsScene * scene);
    void setRotate(int angle);
public slots:
    void robotStep();
};


#endif // ROBOT_H
