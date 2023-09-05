#include "floor_sensor.h"

Floor_Sensor::Floor_Sensor(){}

Floor_Sensor::Floor_Sensor(int floor){
    sensor_floor_pos = floor;
}

bool Floor_Sensor::DetectCar(Elevator_Car* elevator){
    std::vector<int> FloorQueue = elevator->GetQueue();

    if (std::find(FloorQueue.begin(), FloorQueue.end(), sensor_floor_pos) == FloorQueue.end()){
        return false;
    } else {
        return true;
    }
}
