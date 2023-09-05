#ifndef ELEVATOR_DOORS_SENS_H
#define ELEVATOR_DOORS_SENS_H

#include "defs.h"

class Elevator_Doors_Sensors {
    public:
    //Constructors
        Elevator_Doors_Sensors();
        Elevator_Doors_Sensors(int);
    //Functions
        //Check for blockage. Check if the passengers in the elevator are blocking the door
        bool DetectBlockage();

        void SetBlockage(bool bl){blockage = bl;}
        void SetOn(bool on){is_on = on;}

    private:
        int elevatorID;
        bool is_on;
        bool blockage;
};
#endif
