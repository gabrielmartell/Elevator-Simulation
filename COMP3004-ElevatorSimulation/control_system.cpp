#include "control_system.h"

using namespace std;

Control_System::Control_System(){}

Control_System::Control_System(int elevato, int floors){
    emergency_mode = false;

    for (int i = 0; i < elevato; ++i){
        Elevator_Car* car = new Elevator_Car(i);
        elevators.push_back(car);
    }
    for (int i = 0; i < floors; ++i){
        Floor_Sensor* sensor = new Floor_Sensor(i);
        Floor_Control_Panel* control_panel = new Floor_Control_Panel(i);
        fss.push_back(sensor);
        fcps.push_back(control_panel);
    }
}

std::string Control_System::ReadSignal(std::string signal, int desired_floor, int elevatorID){

    int direction;
    Elevator_Car* elevator = elevators[elevatorID];

    /*MOVE CAR SIGNAL
    ------------------------------------------*/
    if (signal == "Pending"){
        if (elevator->Overload()){return "Overload";}
        if (elevator->doors.sensors.DetectBlockage() == true){return "Blockage";}
        if (elevator->GetCurrentFloor() == desired_floor-1){
            std::cout << "[TERMINAL] Elevator ID#"<< elevatorID << " Car: Already on Floor " << desired_floor << std::endl;
            return "SameFloor";
        }
        return "MoveCar";
    }

    else if (signal == "MoveCar"){
        //std::cout << "[SIGNAL] MoveCar" << std::endl;

        //IF THE ELEVATOR IS IDLE
        if (!elevator->InTransit()){
            if (elevator->GetCurrentFloor() < desired_floor-1){direction = 1;}
            else{direction = 0;}
            elevator->SetDirection(direction);
            elevator->SetTransit(true);
        }

        //IF THE ELEVATOR IS AT THE FLOOR IT WAS CALLED AT
        if (elevator->GetCurrentFloor() == desired_floor-1){
            std::cout << "[TERMINAL] Elevator ID#"<< elevatorID << " Car: Already on Floor " << desired_floor << std::endl;
            return "SameFloor";
        }

        //OTHERWISE, MOVE THE ELEVATOR
        elevator->AddToQueue(desired_floor-1);
        return MoveCar(elevatorID);

    /*STOP CAR SIGNAL
    ------------------------------------------*/
    } else if (signal == "StopCar"){
        //std::cout << "[SIGNAL] StopCar" << std::endl;
        std::cout << "[TERMINAL] Elevator ID#"<< elevatorID << " Car: Stopping at " << desired_floor << std::endl;
        StopCar(elevatorID);

    /*CAR DOORS SIGNALS
    ------------------------------------------*/
    } else if (signal == "OpenDoors"){
        //std::cout << "[SIGNAL] OpenDoors" << std::endl;
        std::cout << "[TERMINAL] Elevator ID#"<< elevatorID << " Doors: Open " << std::endl;
        OpenDoors(elevatorID, "NULL");

    } else if (signal == "CloseDoors"){
        //std::cout << "[SIGNAL] CloseDoors" << std::endl;
        CloseDoors(elevatorID);
    /*SAFETY SIGNALS
    ------------------------------------------*/
    } else if (signal == "Overload"){
        StopCar(elevatorID);
        OpenDoors(elevatorID, signal);

    } else if (signal == "PowerOutage"){
        UseBackupBattery();
        EnterEmergencyMode();
        std::cout << "[TERMINAL] Elevator ID#" << elevatorID << " has Entered Emergency Mode" << std::endl;

        elevator->audio_sys.PlayEmergencyAudio();
        elevator->display_sys.DisplayMessage("The building has lost power, please stay calm and exit the elevator when it comes to a stop.");
        std::cout << "[TERMINAL] CONTROL SYSTEM: The Control System is now Using the Backup Battery - providing power to Elevator ID#" << elevatorID << std::endl;
        return "MoveCar";

    } else if (signal == "Fire"){
        EnterEmergencyMode();
        std::cout << "[TERMINAL] Elevator ID#" << elevatorID << " has Entered Emergency Mode" << std::endl;
        elevator->audio_sys.PlayEmergencyAudio();
        elevator->display_sys.DisplayMessage("The building's fire alarm has been triggered, please stay calm and exit the elevator when it comes to a stop.");
        return "MoveCar";

    } else if (signal == "Help"){
        StopCar(elevatorID);
        ConnectToHelp(elevatorID);
        ConnectPhoneAudio(elevatorID);
        elevator->SetActive(false);

;    } else if (signal == "Blockage"){
        StopCar(elevatorID);
        OpenDoors(elevatorID, signal);
    }
    return "Idle";
}

//Find Closest Elevator
int Control_System::FindClosestElevator(int current_floor, int direction){
    //Check status of all elevators, that being, if all are out of service, return -1
    for (Elevator_Car* car : elevators){
        if (car->IsActive()){
            break;
        }
        if (car->GetElevatorID() == (int)elevators.size()-1){
            std::cout << "[TERMINAL] All elevators are Out of Order/Unavailable as of right now." << std::endl;
            return -1;
        }
    }
    /*We want to consider all elevators in transit - that is:
        If an elevator is in transit and is going at the direction we want, check.
        If said elevator is going to intersect with our floor, check.
        If the list is empty, the passenger will then have to wait for an available elevator - therefore just find the closest elevator that is NOT in transit.
    */
    std::vector<Elevator_Car*> elevatorsInTransit;

    for (Elevator_Car* car : elevators){
        if (car->GetDirection() == direction && car->InTransit() && car->IsActive()){
            if (direction == 1){ //IF THE ELEVATOR IS GOING UP
                if (car->GetCurrentFloor() < current_floor){
                    elevatorsInTransit.push_back(car);
                }
            } else { //IF THE ELEVATOR IS GOING DOWN
                if (car->GetCurrentFloor() > current_floor){
                    elevatorsInTransit.push_back(car);
                }
            }
        }
    }

    //IF THERE IS ONLY 1 ELEVATOR IN TRANSIT THAT WILL INTERSECT
    if (elevatorsInTransit.size() == 1){
        std::cout << "[DEBUG] Only one elevator in transit that intersects with Floor " << current_floor << std::endl;
        return elevatorsInTransit[0]->GetElevatorID();

    //IF THERE IS ARE MULTIPLE ELEVATORS IN TRANSIT THAT WILL INTERSECT
    } else if (elevatorsInTransit.size() > 1){

        std::cout << "[DEBUG] Multiple elevators in transit that intersects with Floor " << current_floor << std::endl;
        //Initialize the shorest car and distance. At the base case, the elevator with ID 0 will be "closest".
        Elevator_Car* closest_car = elevators[0];
        int shortest_distance;
        if (closest_car->GetCurrentFloor() > current_floor){
            shortest_distance = closest_car->GetCurrentFloor() - current_floor;
        } else {
            shortest_distance = current_floor - closest_car->GetCurrentFloor();
        }

        for (Elevator_Car* car : elevatorsInTransit){
            int distance;
            if (car->GetCurrentFloor() > current_floor){
                distance = car->GetCurrentFloor() - current_floor;
            } else {
                distance = current_floor - car->GetCurrentFloor();
            }

            if (distance < shortest_distance){
                shortest_distance = distance;
                closest_car = car;
            }
        }

        return closest_car->GetElevatorID();

    //IF THERE ARE NO ELEVATORS IN TRANSIT, JUST FIND THE CLOSEST ONE IN GENERAL.

    } else {
        //std::cout << "[DEBUG] Multiple elevators not in transit and can go to Floor " << current_floor << std::endl;

        //Initialize the shorest car and distance. At the base case, the elevator with ID 0 will be "closest".
        Elevator_Car* closest_car;

        for (Elevator_Car* car : elevators){
            if (car->IsActive() && !car->doors.sensors.DetectBlockage()){
                closest_car = car;
                break;
            }
        }

        int shortest_distance;
        if (closest_car->GetCurrentFloor() > current_floor){
            shortest_distance = closest_car->GetCurrentFloor() - current_floor;
        } else {
            shortest_distance = current_floor - closest_car->GetCurrentFloor();
        }

        for (Elevator_Car* car : elevators){
            int distance;
            if (car->IsActive() && !car->doors.sensors.DetectBlockage()){
                if (car->GetCurrentFloor() > current_floor){
                    distance = car->GetCurrentFloor() - current_floor;
                } else {
                    distance = current_floor - car->GetCurrentFloor();
                }

                if (distance < shortest_distance){
                    shortest_distance = distance;
                    closest_car = car;
                }
            }
        }
        return closest_car->GetElevatorID();
    }
    return 0;
}

std::string Control_System::MoveCar(int elevatorID){
    Elevator_Car* elevator = elevators[elevatorID];
    std::vector<int> FloorQueue = elevator->GetQueue();

    elevator->SetCurrentFloor();
    bool destination = fss[elevator->GetCurrentFloor()]->DetectCar(elevator);

    if (destination == false){
         std::cout << "[TERMINAL] Elevator ID#"<< elevatorID << " Car: Moving to Floor " << elevator->GetCurrentFloor()+1 << std::endl;
        elevator->SetTransit(true);
        return "MoveCar";
    } else {
        std::cout << "[TERMINAL] Elevator ID#"<< elevatorID << " Car: Stopping/Arriving at Floor " << elevator->GetCurrentFloor()+1 << std::endl;

        if (emergency_mode){
            elevator->ClearQueue();
        }
        return "StopCar";
    }
}

void Control_System::StopCar(int elevatorID){
    Elevator_Car* elevator = elevators[elevatorID];
    elevator->SetTransit(false);
    elevator->RemoveFromQueue(elevator->GetCurrentFloor());
}

//Open the elevator doors
void Control_System::OpenDoors(int elevatorID, std::string sig){
    Elevator_Car* elevator = elevators[elevatorID];

    if (sig == "NULL"){
        elevator->doors.SetDoorsOpen(true);
        elevator->audio_sys.RingBell();
        std::string signal;
        //Let this represent the amount of time the doors are open/sensors are on, it's theoretically set to 10 seconds - and dynamically checking the sensors.
        for (int i = 0; i < 10; ++i){
            signal = elevator->doors.ActivateSensors();
        }
        elevator->audio_sys.StopAudio();
        ReadSignal(signal, 0, elevatorID);

    } else if (sig == "Overload"){
        elevator->doors.SetDoorsOpen(true);
        elevator->audio_sys.PlayOverloadAudio();
        elevator->display_sys.DisplayMessage("The current elevator weight exceeds the capacity, please remove some weight and wait.");
        std::cout << "[TERMINAL] Elevator ID#"<< elevatorID << " Doors: Remain Open" << std::endl;

    } else if (sig == "Blockage"){
        elevator->doors.SetDoorsOpen(true);
        elevator->audio_sys.PlayWarningAudio();
        elevator->display_sys.DisplayMessage("The doors are blocked, please move away all hazards from the doors.");
        std::cout << "[TERMINAL] Elevator ID#"<< elevatorID << " Doors: Remain Open" << std::endl;

    } else if (sig == "Button"){
        elevator->doors.SetDoorsOpen(true);
        std::cout << "[TERMINAL] Elevator ID#"<< elevatorID << " Doors: Remain Open" << std::endl;
    }
}

//Close the elevator doors
void Control_System::CloseDoors(int elevatorID){
    Elevator_Car* elevator = elevators[elevatorID];

    if (elevator->doors.sensors.DetectBlockage()){
        elevator->doors.SetDoorsOpen(true);
        elevator->audio_sys.RingBell();
        std::cout << "[TERMINAL] Elevator ID#"<< elevatorID << " Doors: Remain Open - Blockage Detected" << std::endl;
        return;

    }
    if (elevator->doors.IsDoorsOpen()){
        elevator->doors.DeactiveSensors();
        elevator->doors.SetDoorsOpen(false);
        elevator->audio_sys.RingBell();
        elevator->SetTransit(false);
        std::cout << "[TERMINAL] Elevator ID#"<< elevatorID << " Doors: Close" << std::endl;
    } else {
        std::cout << "[TERMINAL] Elevator ID#"<< elevatorID << " Doors: Already Closed" << std::endl;
        elevator->doors.SetDoorsOpen(false);
    }
    elevator->audio_sys.StopAudio();

    if (elevator->GetQueue().size() <= 0){
        ReadSignal("Idle", 0, elevatorID);
    }
}


//Call the building safety service
void Control_System::ConnectToHelp(int elevatorID){
    std::cout << "[TERMINAL] Control System is now contacting Safety Services for Elevator ID#" << elevatorID << std::endl;
}

//Connect the phone call's audio to the elevator's sound system
void Control_System::ConnectPhoneAudio(int elevatorID){
    std::cout << "[TERMINAL] Control System is now connecting Phone Call with Safety Services for Elevator ID#" << elevatorID << std::endl;
}

//Activate emergency mode for all elevators

void Control_System::EnterEmergencyMode(){
    emergency_mode = true;
}
//Activate the backup battery for all elevators
void Control_System::UseBackupBattery(){
    backup_battery_in_use = true;
}

