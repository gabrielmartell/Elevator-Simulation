#include "elevator_doors.h"

Elevator_Doors::Elevator_Doors(){}

Elevator_Doors::Elevator_Doors(int id){
    elevatorID = id;
    is_open = false;
}

std::string Elevator_Doors::ActivateSensors(){
    sensors.SetOn(true);

    if (sensors.DetectBlockage()){
        return "Blockage";
    } else {
        return "CloseDoors";
    }
}

void Elevator_Doors::DeactiveSensors(){
    sensors.SetOn(false);
}

