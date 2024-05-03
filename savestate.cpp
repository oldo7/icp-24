#include "savestate.h"
#include "addrobot.h"
#include "qevent.h"
#include "qlabel.h"
#include "simulation.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QIODevice>
#include <QStringEncoder>
#include <QStringView>
#include <iostream>
#include <string_view>
#include <string>

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
    QXmlStreamWriter stream(&file);

    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("elements");

    //zapis aktualnych parametrov robotov do suboru
    std::list<Robot*>::iterator itr = robotList.begin();

    for(itr; itr!=robotList.end(); itr++){
        stream.writeStartElement("robot");
        stream.writeTextElement("x", std::to_string((*itr)->x()));
        stream.writeTextElement("y", std::to_string((*itr)->y()));
        stream.writeTextElement("angle", std::to_string((*itr)->angle));
        stream.writeTextElement("scan_size", std::to_string((*itr)->scanSize));
        stream.writeTextElement("rotate_angle", std::to_string((*itr)->rotateAngle));
        stream.writeEndElement();
    }

    //zapis parametrov kontrolovatelneho robota do suboru (ak exustuje)
    if (addRobot->contRobot != nullptr){
        stream.writeStartElement("controllable");
        stream.writeTextElement("x", std::to_string(addRobot->contRobot->x()));
        stream.writeTextElement("y", std::to_string(addRobot->contRobot->y()));
        stream.writeTextElement("angle", std::to_string(addRobot->contRobot->angle));
        stream.writeTextElement("scan_size", std::to_string(addRobot->contRobot->scanSize));
        stream.writeEndElement();
    }

    //zapis obstaclov
    std::list<QGraphicsRectItem*>::iterator gitr = obstacleList.begin();

    for(gitr; gitr!=obstacleList.end(); gitr++){

        //zistenie velkosti obstaclu
        qreal a, b, c, d;
        (*gitr)->boundingRect().getRect(&a, &b, &c, &d);
        int size = (a-c)*(-1) - 1.5;
        if(size<0){
            size = c-1;
        }
        qDebug() << a << b << c << d;
        qDebug() << "zapisujem size " << size;


        //zapis do XML
        stream.writeStartElement("obstacle");
        stream.writeTextElement("x", std::to_string(static_cast<int>((*gitr)->pos().x())));
        stream.writeTextElement("y", std::to_string(static_cast<int>((*gitr)->pos().y())));
        stream.writeTextElement("size", std::to_string(size));
        stream.writeEndElement();
    }

    stream.writeEndElement();
    stream.writeEndDocument();
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

    //remove all objects from the scene
    simulation->close();
    simulation = new Simulation();
    simulation->show();

    //remove all objects from internal lists
    obstacleList.clear();
    robotList.clear();
    addRobot->contRobot = nullptr;

    //load objects into internal lists and into the scene
    QXmlStreamReader xml(&file);

    xml.readNextStartElement();
    xml.readNextStartElement();


    std::wstring_view wview(L"obstacle");
    QStringView obst(wview.data(), wview.data() + wview.size());
    std::wstring_view wview2(L"robot");
    QStringView rob(wview2.data(), wview2.data() + wview2.size());
    std::wstring_view wview3(L"controllable");
    QStringView contr(wview3.data(), wview3.data() + wview3.size());
    std::wstring_view wview4(L"x");
    QStringView vx(wview4.data(), wview4.data() + wview4.size());
    std::wstring_view wview5(L"y");
    QStringView vy(wview5.data(), wview5.data() + wview5.size());
    std::wstring_view wview6(L"angle");
    QStringView vangle(wview6.data(), wview6.data() + wview6.size());
    std::wstring_view wview7(L"scan_size");
    QStringView vscan_size(wview7.data(), wview7.data() + wview7.size());
    std::wstring_view wview8(L"rotate_angle");
    QStringView vrotate_angle(wview8.data(), wview8.data() + wview8.size());
    std::wstring_view wview9(L"size");
    QStringView vsize(wview9.data(), wview9.data() + wview9.size());
    /*
    if(qstrv2b == xml.name()){
        qDebug() << "equal";
    }else{
        qDebug() << "not equal";
    }
    xml.readNextStartElement();
    qDebug() << xml.name();
    qDebug() << xml.readElementText().toFloat();
    */

    //TODO:

    qDebug() << "XML read";
    while(!xml.atEnd()){

        if(xml.name() == rob){                  //robot
            qDebug() << "found a robot";
            double x = -1;
            double y = -1;
            double rotate_angle = -1;
            bool rotate_angle_defined = false;
            double scan_size = -1;
            double angle = -1;
            bool angle_defined = false;
            for(int i=0; i<5; i++){             //load individual values for robot from XML
                xml.readNextStartElement();
                if(xml.name() == vx){
                    x = xml.readElementText().toDouble();
                }
                if(xml.name() == vy){
                    y = xml.readElementText().toDouble();
                }
                if(xml.name() == vangle){
                    angle = xml.readElementText().toDouble();
                    angle_defined = true;
                }
                if(xml.name() == vscan_size){
                    scan_size = xml.readElementText().toDouble();
                }
                if(xml.name() == vrotate_angle){
                    rotate_angle = xml.readElementText().toDouble();
                    rotate_angle_defined = true;
                }
            }
            //qDebug() << "x: " << x << "y: " << y << "angle " << angle << "scan size" << scan_size << "rotate_angle" <<rotate_angle;
            if(!rotate_angle_defined || !angle_defined || x == -1 || y==-1 || scan_size==-1){
                std::cerr << "Error parsing XML file";
                return;
            }
            Robot * newRobot = new Robot(0, x, y, scan_size, rotate_angle, true, angle, simulation->scene);
            simulation->scene->addItem(newRobot);
            robotList.push_back(newRobot);
            xml.readNextStartElement();
        }

        if(xml.name() == obst){                  //obstacle
            qDebug() << "found an obstacle";
            int x = -1;
            int y = -1;
            int size = -1;
            for(int i=0; i<3; i++){             //load individual values for robot from XML
                xml.readNextStartElement();
                if(xml.name() == vx){
                    x = xml.readElementText().toInt();
                }
                if(xml.name() == vy){
                    y = xml.readElementText().toInt();
                }
                if(xml.name() == vsize){
                    size = xml.readElementText().toInt();
                }
            }
            if(x == -1 || y==-1 || size==-1){
                std::cerr << "Error parsing XML file";
                return;
            }
            qDebug() << "read size = " << size;
            QGraphicsRectItem * newObstacle = new QGraphicsRectItem();
            newObstacle->setRect(x,y,size,size);
            newObstacle->setPos(x,y);
            obstacleList.push_back(newObstacle);

            QGraphicsRectItem * newObstacle2 = new QGraphicsRectItem();
            newObstacle2->setRect(x,y,size,size);
            simulation->scene->addItem(newObstacle2);


            xml.readNextStartElement();
        }

        if(xml.name() == contr){                  //controllable robot
            qDebug() << "found a controllable robot";
            double x = -1;
            double y = -1;
            double scan_size = -1;
            double angle = -1;
            bool angle_defined = false;
            for(int i=0; i<4; i++){             //load individual values for robot from XML
                xml.readNextStartElement();
                if(xml.name() == vx){
                    x = xml.readElementText().toDouble();
                }
                if(xml.name() == vy){
                    y = xml.readElementText().toDouble();
                }
                if(xml.name() == vangle){
                    angle = xml.readElementText().toDouble();
                    angle_defined = true;
                }
                if(xml.name() == vscan_size){
                    scan_size = xml.readElementText().toDouble();
                }
            }
            //qDebug() << "x: " << x << "y: " << y << "angle " << angle << "scan size" << scan_size << "rotate_angle" <<rotate_angle;
            if(!angle_defined || x == -1 || y==-1 || scan_size==-1){
                std::cerr << "Error parsing XML file";
                return;
            }
            addRobot->contRobot = new ControllableRobot(0, x, y, scan_size, simulation->scene);
            simulation->scene->addItem( addRobot->contRobot);

            xml.readNextStartElement();
        }

        xml.readNextStartElement();
    }

    std::list<QGraphicsRectItem*>::iterator gitr = obstacleList.begin();

    for(gitr; gitr!=obstacleList.end(); gitr++){

        //zistenie velkosti obstaclu
        qDebug() << "obstacle X : " << (*gitr)->x();
    }

}


