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
    double angleToRotate;
    void move();
    void rotate();
public:
    Robot(QGraphicsItem *parent, int initX, int initY, int scanAreaSize, int rotateAngle, bool rotateClockwise, QGraphicsScene * scene);
    Robot(QGraphicsItem *parent, int initX, int initY, int scanAreaSize, QGraphicsScene * scene);
    void setRotate(int angle);
public slots:
    void robotStep();
};


#endif // ROBOT_H
