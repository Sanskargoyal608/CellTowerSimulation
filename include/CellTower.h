#ifndef CELL_TOWER_H
#define CELL_TOWER_H

#include "CellularCore.h"
#include "UserDevice.h"

class CellTower {
protected:
    int towerId;
    CellularCore* core; 

public:
    CellTower(int id, CellularCore* c) : towerId(id), core(c) {}
    virtual ~CellTower() {}

    // These Pure Virtual functions are the "Contract" for Phase 4
    virtual int calculateMaxCapacity() = 0; 
    virtual bool addUser(UserDevice* user) = 0;
    
    int getId() const { return towerId; }
};

#endif