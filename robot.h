#ifndef ROBOT_H
#define ROBOT_H
#include "scanAreaT.h"
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>

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
    Robot(QGraphicsItem *parent, double initX, double initY, int scanAreaSize, int rotateAngle, bool rotateClockwise, double initRotate, QGraphicsScene * scene);
    Robot(QGraphicsItem *parent, double initX, double initY, int scanAreaSize, QGraphicsScene * scene);
    void setRotate(int angle);
public slots:
    void robotStep();
};


#endif // ROBOT_H
