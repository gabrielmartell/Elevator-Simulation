#include "elevator_doors_sensors.h"

Elevator_Doors_Sensors::Elevator_Doors_Sensors(){}

Elevator_Doors_Sensors::Elevator_Doors_Sensors(int id){
    blockage = false;
    elevatorID = id;
}

bool Elevator_Doors_Sensors::DetectBlockage(){
    return blockage;
}
