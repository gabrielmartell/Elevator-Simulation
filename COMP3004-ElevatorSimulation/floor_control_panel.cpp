#include "floor_control_panel.h"

Floor_Control_Panel::Floor_Control_Panel(){}

Floor_Control_Panel::Floor_Control_Panel(int floor){
    floor_pos = floor;
}

std::string Floor_Control_Panel::UpButtonPressed(){
    std::string str = "[Floor Contol Panel " + std::to_string(floor_pos+1) + "] The UP button was pressed.";
    return str;
}
std::string Floor_Control_Panel::DownButtonPressed(){
    std::string str = "[Floor Contol Panel " + std::to_string(floor_pos+1) + "] The DOWN button was pressed.";
    return str;
}

std::string Floor_Control_Panel::IlluminateUpButton(){
    std::string str = "[Floor Contol Panel " + std::to_string(floor_pos+1) + "] The UP button was illuminated.";
    return str;
}
std::string Floor_Control_Panel::IlluminateDownButton(){
    std::string str = "[Floor Contol Panel " + std::to_string(floor_pos+1) + "] The DOWN button was illuminated.";
    return str;
}
