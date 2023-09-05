#include "elevator_car.h"

Elevator_Car::Elevator_Car(){}

Elevator_Car::Elevator_Car(int id){
    elevatorID = id;
    current_floor = 0;
    current_weight = 0;
    weight_limit = 15;

    audio_sys = Audio_System(id);
    doors = Elevator_Doors(id);
    display_sys = Display_System(id);
    active = true;
}

bool Elevator_Car::DetermineLoadWeight(){
    return current_weight > weight_limit;
}

//Add floor/remove to the queue
void Elevator_Car::AddToQueue(int floor){
    if (std::find(FloorQueue.begin(), FloorQueue.end(), floor) == FloorQueue.end()){
        FloorQueue.push_back(floor);
    }
}

void Elevator_Car::ClearQueue(){
    FloorQueue.clear();
}

void Elevator_Car::RemoveFromQueue(int floor){
    if (std::find(FloorQueue.begin(), FloorQueue.end(), floor) != FloorQueue.end()){
        FloorQueue.erase(std::remove(FloorQueue.begin(), FloorQueue.end(), floor), FloorQueue.end());
    }
}

void Elevator_Car::SetCurrentFloor(){
    if (direction == 1){
        current_floor+=1;
    } else {
        current_floor-=1;
    }
}
