#ifndef AUDIO_H
#define AUDIO_H

#include <string>
#include <vector>
#include "defs.h"

class Audio_System {
    public:
    //Constructor
        Audio_System();
        Audio_System(int);
    //Functions
        void RingBell();
        void PlayEmergencyAudio();
        void PlayWarningAudio();
        void PlayOverloadAudio();
        void StopAudio();
        
    private:
        int elevatorID;
};
#endif
