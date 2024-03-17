#ifndef ROBOT_H
#define ROBOT_H
#include "scanAreaT.h"
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>

class Robot:public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
protected:
    double angle;
    scanAreaT * scanArea;
    bool checkNoObstacles();
    int rotateAngle;
public:
    Robot(QGraphicsItem *parent, int initX, int initY, int scanAreaSize, int rotateAngle, bool rotateClockwise, QGraphicsScene * scene);
    Robot(QGraphicsItem *parent, int initX, int initY, int scanAreaSize, QGraphicsScene * scene);
public slots:
    void move();
    void rotate(double angleToRotate);
};


#endif // ROBOT_H
