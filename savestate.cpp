#include "savestate.h"
#include "addrobot.h"
#include "qevent.h"
#include "simulation.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QIODevice>

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
}


void saveState::saveData(){
    remove("test2.txt");
    QFile file("test2.txt");        //TODO: make custom
    file.open(QIODevice::ReadWrite);
    QXmlStreamWriter stream(&file);

    stream.setAutoFormatting(true);
    stream.writeStartDocument();

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
        qreal size = (a-c)*(-1) - 1.5;

        //zapis do XML
        stream.writeStartElement("obstacle");
        stream.writeTextElement("x", std::to_string((*gitr)->pos().x()));
        stream.writeTextElement("y", std::to_string((*gitr)->pos().y()));
        stream.writeTextElement("size", std::to_string(size));
        stream.writeEndElement();
    }


    stream.writeEndDocument();
}

void saveState::loadData(){
    //remove all objects from the scene
    simulation->close();
    simulation = new Simulation();
    simulation->show();

    //remove all objects from internal lists
    obstacleList.clear();
    robotList.clear();
    addRobot->contRobot = nullptr;

    //load objects into internal lists and into the scene
    QFile file("test2.txt");        //TODO: make custom
    file.open(QIODevice::ReadOnly);

    QXmlStreamReader xml(&file);

    //TODO:
    //while(!xml.atEnd())
    qDebug() << "XML READ";
    qDebug() << xml.name();
    xml.readNextStartElement();

    qDebug() << xml.name();
    xml.readNextStartElement();
    qDebug() << xml.name();
    qDebug() << xml.readElementText();

    xml.readNextStartElement();
    qDebug() << xml.name();
    qDebug() << xml.readElementText();
}
