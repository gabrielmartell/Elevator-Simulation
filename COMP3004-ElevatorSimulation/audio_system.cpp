#include "audio_system.h"

Audio_System::Audio_System(){
    //Nothing
}

Audio_System::Audio_System(int id){
    elevatorID = id;
}
void Audio_System::RingBell(){std::cout << "[TERMINAL] Elevator ID#" << elevatorID << " Audio System: Ring Bell" << std::endl;}
void Audio_System::PlayEmergencyAudio(){std::cout << "[TERMINAL] Elevator ID#" << elevatorID << " Audio System: Emergency Warning Audio" << std::endl;}
void Audio_System::PlayWarningAudio(){std::cout << "[TERMINAL] Elevator ID#" << elevatorID << " Audio System: Blockage Audio" << std::endl;}
void Audio_System::PlayOverloadAudio(){std::cout << "[TERMINAL] Elevator ID#" << elevatorID << " Audio System: Overload Audio" << std::endl;}
void Audio_System::StopAudio(){std::cout << "[TERMINAL] Elevator ID#" << elevatorID << " Audio System: Stop Audio" << std::endl;}
