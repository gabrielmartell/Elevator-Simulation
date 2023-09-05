#ifndef ELEVATOR_DOORS_H
#define ELEVATOR_DOORS_H

#include "defs.h"
#include "elevator_doors_sensors.h"

class Elevator_Doors {
    public:
    //Constructor
        Elevator_Doors();
        Elevator_Doors(int);
    //Functions
        //Activiate the door sensors
        std::string ActivateSensors();
        void SetDoorsOpen(bool op) {is_open = op;}

        //Deactivate the door sensors
        void DeactiveSensors();

        bool IsDoorsOpen(){ return is_open; } 

        Elevator_Doors_Sensors sensors;

    private:
        int elevatorID;
        bool is_open;
};
#endif
