#ifndef FLOOR_SENSOR_H
#define FLOOR_SENSOR_H

#include "defs.h"
#include "elevator_car.h"

class Floor_Sensor {
    public:
    //Constructor
        Floor_Sensor();
        Floor_Sensor(int);
    //Functions
        bool DetectCar(Elevator_Car*);
        int GetFloor(){ return sensor_floor_pos; } 
    private:
        int sensor_floor_pos;
};

#endif
