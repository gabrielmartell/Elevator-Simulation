#ifndef FLOOR_CONTROL_PANEL_H
#define FLOOR_CONTROL_PANEL_H

#include "defs.h"

class Floor_Control_Panel {
    public:
    //Constructor
        Floor_Control_Panel();
        Floor_Control_Panel(int);
    //Functions
        //Send a signal "FloorCallUp" or "FloorCallDown" respectively
        std::string UpButtonPressed();
        std::string DownButtonPressed();

        std::string IlluminateUpButton();
        std::string IlluminateDownButton();

        int GetFloor(){ return floor_pos; } 

    private:
        int floor_pos;
};

#endif
