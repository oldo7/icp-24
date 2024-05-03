#include "savestate.h"
#include "addrobot.h"
#include "qevent.h"
#include "qlabel.h"
#include "simulation.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QIODevice>
#include <iostream>
#include <string_view>
#include <string>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

extern std::list<Robot*> robotList;
extern AddRobot * addRobot;
extern std::list<QGraphicsRectItem*> obstacleList;
extern Simulation * simulation;

saveState::saveState(Simulation * simulation) : QObject() {
    sim = simulation;
    saveStateButton = new QPushButton("Save state", simulation);
    saveStateButton->setGeometry(QRect(QPoint(880, 620), QSize(100, 50)));
    connect(saveStateButton, SIGNAL(released()), this, SLOT(saveData()));

    saveStateButton = new QPushButton("Load state", simulation);
    saveStateButton->setGeometry(QRect(QPoint(880, 680), QSize(100, 50)));
    connect(saveStateButton, SIGNAL(released()), this, SLOT(loadData()));

    QLabel * controllableGroupLabel = new QLabel(simulation);
    controllableGroupLabel->setGeometry(QRect(QPoint(887, 740), QSize(150, 20)));
    controllableGroupLabel->setStyleSheet("font-size: 15px");
    controllableGroupLabel->setText("From/To File:");

    fileName = new QLineEdit(simulation);
    fileName->setGeometry(QRect(QPoint(870, 760), QSize(120, 30)));
    fileName->setEchoMode(QLineEdit::Normal);
    fileName->setPlaceholderText("Filename");
    fileName->setText("config.xml");

    nonExistentFile = new QLabel(simulation);
    nonExistentFile->setGeometry(QRect(QPoint(880, 788), QSize(170, 50)));
    nonExistentFile->setStyleSheet("color: white;");
    nonExistentFile->setText("Specified file does \n        not exist");
}


void saveState::saveData(){
    std::string filestr = fileName->text().toStdString();
    const char * filen = filestr.c_str();
    remove(filen);
    QFile file(filen);        //TODO: make custom
    file.open(QIODevice::ReadWrite);

    //zapis aktualnych parametrov robotov do suboru
    std::list<Robot*>::iterator itr = robotList.begin();


    QJsonArray jsonArray;
    QJsonObject root;


    for(itr; itr!=robotList.end(); itr++){
        qDebug() << "saving robot";
        QJsonObject jsonObject;
        QJsonObject robotParams;

        robotParams.insert("x",(*itr)->x());
        robotParams.insert("y",(*itr)->y());
        robotParams.insert("angle",(*itr)->angle);
        robotParams.insert("scan_size",(*itr)->scanSize);
        robotParams.insert("rotate_angle",(*itr)->rotateAngle);
        jsonObject.insert("Robot",robotParams);
        jsonArray.append(jsonObject);
    }

    //zapis parametrov kontrolovatelneho robota do suboru (ak exustuje)
    if (addRobot->contRobot != nullptr){
        qDebug() << "saving controllable";

        QJsonObject jsonObject2;
        QJsonObject contRobotParams;

        contRobotParams.insert("x",addRobot->contRobot->x());
        contRobotParams.insert("y",addRobot->contRobot->y());
        contRobotParams.insert("angle",addRobot->contRobot->angle);
        contRobotParams.insert("scan_size",addRobot->contRobot->scanSize);
        jsonObject2.insert("Controllable",contRobotParams);
        jsonArray.append(jsonObject2);
    }

    //zapis obstaclov
    std::list<QGraphicsRectItem*>::iterator gitr = obstacleList.begin();

    for(gitr; gitr!=obstacleList.end(); gitr++){
        qDebug() << "saving obstacles";

        //zistenie velkosti obstaclu
        qreal a, b, c, d;
        (*gitr)->boundingRect().getRect(&a, &b, &c, &d);
        int size = (a-c)*(-1) - 1.5;
        if(size<0){
            size = c-1;
        }
        qDebug() << a << b << c << d;
        qDebug() << "zapisujem size " << size;

        QJsonObject jsonObject3;
        QJsonObject obsParams;

        obsParams.insert("x",(*gitr)->x());
        obsParams.insert("y",(*gitr)->y());
        obsParams.insert("size",size);
        jsonObject3.insert("Obstacle",obsParams);
        jsonArray.append(jsonObject3);
    }
    QJsonDocument jsonDoc;
    root.insert("root",jsonArray);
    jsonDoc.setObject(root);
    file.write(jsonDoc.toJson());
    file.close();

    nonExistentFile->setStyleSheet("color: white;");
}

void saveState::loadData(){
    nonExistentFile->setStyleSheet("color: red;");

    std::string filestr = fileName->text().toStdString();
    const char * filen = filestr.c_str();
    QFile file(filen);
    if(!file.exists()){
        qDebug() << "Zadany subor neexistuje";

        return;
    }
    file.open(QIODevice::ReadOnly);

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(data, &errorPtr);
    if (doc.isNull()) {
        qDebug() << "Parse failed";
    }


    QJsonObject Obj = doc.object();
    QJsonArray rootArr = Obj.value("root").toArray();

    //remove all objects from the scene
    simulation->close();
    simulation = new Simulation();
    simulation->show();

    //remove all objects from internal lists
    obstacleList.clear();
    robotList.clear();
    addRobot->contRobot = nullptr;

    for (auto elements : rootArr){
        qDebug() << "------------------";
        QJsonObject objPoint = elements.toObject();

        if(!objPoint.value("Obstacle").isUndefined()){                          //reading an obstacle
            QJsonObject obstObj = objPoint.value("Obstacle").toObject();

            if(obstObj.value("x").isUndefined() ||obstObj.value("y").isUndefined() || obstObj.value("size").isUndefined()){
                std::cerr << "error parsing obstacle from JSON";
                return;
            }
            int x = obstObj.value("x").toInt();
            int y = obstObj.value("y").toInt();
            int size = obstObj.value("size").toInt();


            QGraphicsRectItem * newObstacle = new QGraphicsRectItem();
            newObstacle->setRect(x,y,size,size);
            newObstacle->setPos(x,y);
            obstacleList.push_back(newObstacle);

            QGraphicsRectItem * newObstacle2 = new QGraphicsRectItem();
            newObstacle2->setRect(x,y,size,size);
            simulation->scene->addItem(newObstacle2);
        }


        if(!objPoint.value("Robot").isUndefined()){                          //reading a robot
            QJsonObject robObj = objPoint.value("Robot").toObject();

            if(robObj.value("x").isUndefined() ||robObj.value("y").isUndefined() || robObj.value("angle").isUndefined()|| robObj.value("scan_size").isUndefined()|| robObj.value("rotate_angle").isUndefined()){
                std::cerr << "error parsing robot from JSON";
                return;
            }
            double x = robObj.value("x").toDouble();
            double y = robObj.value("y").toDouble();
            double rotate_angle = robObj.value("rotate_angle").toDouble();
            double scan_size = robObj.value("scan_size").toDouble();
            double angle = robObj.value("angle").toDouble();

            Robot * newRobot = new Robot(0, x, y, scan_size, rotate_angle, true, angle, simulation->scene);
            simulation->scene->addItem(newRobot);
            robotList.push_back(newRobot);
        }

        if(!objPoint.value("Controllable").isUndefined()){              //reading a controllable robot
            QJsonObject contObj = objPoint.value("Controllable").toObject();

            if(contObj.value("x").isUndefined() ||contObj.value("y").isUndefined() || contObj.value("angle").isUndefined()|| contObj.value("scan_size").isUndefined()){
                std::cerr << "error parsing controllable robot from JSON";
                return;
            }
            double x = contObj.value("x").toDouble();
            double y = contObj.value("y").toDouble();
            double scan_size = contObj.value("scan_size").toDouble();
            double angle = contObj.value("angle").toDouble();

            addRobot->contRobot = new ControllableRobot(0, x, y, scan_size, simulation->scene);
            simulation->scene->addItem( addRobot->contRobot);
        }
    }
}


