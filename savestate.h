#ifndef SAVESTATE_H
#define SAVESTATE_H

#include "qobject.h"
#include "simulation.h"

class saveState  : QObject
{
    Q_OBJECT
public:
    saveState(Simulation * simulation);
    QPushButton * saveStateButton;
public slots:
    void saveData();
};

#endif // SAVESTATE_H
