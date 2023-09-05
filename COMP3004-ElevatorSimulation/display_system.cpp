#include "display_system.h"

Display_System::Display_System(){}

Display_System::Display_System(int id){
    elevatorID = id;
}

void Display_System::DisplayMessage(std::string str){std::cout << "[TERMINAL] Elevator ID#" << elevatorID << " Display System: " << str << std::endl;}
