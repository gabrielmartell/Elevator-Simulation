#ifndef CONTROL_SYSTEM_H
#define CONTROL_SYSTEM_H

#include "defs.h"
#include "floor_control_panel.h"
#include "floor_sensor.h"
#include "elevator_car.h"

class Control_System {
    public:

    //Constructor
        Control_System();
        Control_System(int el, int fl);
    
    //Functons
        /*
        Read the signal, check the signal, and respond accordingly depending on the signal through a series of cases. 
        The desired floor parameter acts as a movement call TO a floor, not from.
        */
        std::string ReadSignal(std::string signal, int desired_floor, int current_floor);

        /*
        Iterate through the elevators and determine the closest elevator that is going in the direction decided (1, 2) or is idle (0)
        If the closest elevator is NOT in transit, use that elevator
        Otherwise, check the next closest. If the next closest is also in transit, use the first closest.
        Otherwise, use the second closest.
        Return the elevator's ID
        */ 
        int FindClosestElevator(int current_floor, int direction);

        /*
        Move a given elevator, and set the elevator's direction
        Increase/Decrease the elevator's current floor by 1
        */
        std::string MoveCar(int elevatorID);
        
        //Stop the car's transit (bool in_transit)
        void StopCar(int elevatorID);

        //Open the elevator doors
        void OpenDoors(int elevatorID, std::string);

        //Close the elevator doors
        void CloseDoors(int elevatorID);

        //Call the building safety service
        void ConnectToHelp(int elevatorID);

        //Connect the phone call's audio to the elevator's sound system
        void ConnectPhoneAudio(int elevatorID);

        //Activate emergency mode for all elevators
        void EnterEmergencyMode();

        //Activate the backup battery for all elevators
        void UseBackupBattery();

        int GetSafeFloor(){return designated_safe_floor;}

        std::vector<Floor_Control_Panel*> fcps;
        std::vector<Floor_Sensor*> fss;
        std::vector<Elevator_Car*> elevators;

    private:
        int designated_safe_floor;

        //All vectors are components connected to the system itself. The index or the floor sensor and controol panel will represent their level in the building
        bool emergency_mode;
        bool backup_battery_in_use;

};

#endif
