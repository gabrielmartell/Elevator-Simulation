#ifndef DISPLAY_H
#define DISPLAY_H

#include "defs.h"

class Display_System {
    public:
    //Constructor
        Display_System();
        Display_System(int);
    //Functions
        //For both following functions, it replaces the "message" with either the floor number or given parameter message respectively
        void DisplayMessage(std::string message);

    private:
        int elevatorID;
};
#endif
