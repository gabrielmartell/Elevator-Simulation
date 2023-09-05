#ifndef BUILDING_H
#define BUILDING_H

#include "control_system.h"
#include "elevator_car.h"

class Building {
    public:
    //Constructor
        Building();
        Building(int);
        
    //Functions
        std::string FireAlarmPressed(){return "Fire";}
        int GetSafeFloor() {return safe_floor;}

        Control_System* cs;

    private:
        int safe_floor;
};
#endif
