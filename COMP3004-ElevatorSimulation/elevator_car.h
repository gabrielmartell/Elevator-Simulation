#ifndef ELEVATOR_CAR_H
#define ELEVATOR_CAR_H

#include "defs.h"
#include "audio_system.h"
#include "display_system.h"
#include "elevator_doors.h"


class Elevator_Car {
    public:
    //Constructor
        Elevator_Car();
        Elevator_Car(int);
    //Functions
        /*
        Determine the load weight. Iterate through all passengers and get the accumulated weight.
        If the load weight is larger than the weight limit, send signal "Overload"
        Otherwise, send signal "Throw"
        */
        bool DetermineLoadWeight();
    
        //Add floor/remove to the queue
        void AddToQueue(int floor);
        void ClearQueue();
        void RemoveFromQueue(int floor); 

        //Weight Check
        bool Overload(){
            return current_weight >= weight_limit;
        }

    //Setters
        //Based on direction, add or remove 1 from the current_floor
        void SetCurrentFloor();
        void SetDirection(int dir){ direction = dir; }
        void SetTransit(bool i) {in_transit = i;}
        void SetCurrentWeight(int we){current_weight = we;}
        void SetActive(bool a){
            active = a;
            std::cout << "[TERMINAL] Elevator ID#" << elevatorID << " is No Longer Active" << std::endl;
        }

    //Getters
        int GetElevatorID(){ return elevatorID; }
        int GetCurrentFloor(){ return current_floor; } 
        int GetDirection(){ return direction; }
        int GetCurrentWeight() { return current_weight;}
        std::vector<int> GetQueue(){ return FloorQueue; }
        bool InTransit(){ return in_transit; }
        bool IsActive() { return active; }

        Audio_System audio_sys;
        Display_System display_sys;
        Elevator_Doors doors;

    private:
        int elevatorID;
        int current_floor;
        int direction; //Direction will be defined as 0 (idle), 1 (down), 2 (up)
        int weight_limit;
        int current_weight;

        bool active;
        bool in_transit;

        std::vector<int> FloorQueue;
};
#endif
