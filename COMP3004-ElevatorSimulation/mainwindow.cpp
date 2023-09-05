#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->upDownButtonsFrame->setHidden(1);
    ui->elevator_informationFrame->setHidden(1);
    ui->elevator_floorButtonsFrame->setHidden(1);
    ui->elevator_otherButtonsFrame->setHidden(1);
    ui->elevator_externalButtons->setHidden(1);

    //BUTTON CONNECTING
    connect(ui->start_button, SIGNAL(released()), this, SLOT (StartSimulation())); //START BUTTON

    //FLOOR CONTROL PANEL
    connect(ui->up_button, SIGNAL(released()), this, SLOT (UpButtonHandle())); //UP BUTTON
    connect(ui->down_button, SIGNAL(released()), this, SLOT (DownButtonHandle())); //DOWN BUTTON

    //CAR CONTROL PANEL
    connect(ui->open_door_button, SIGNAL(pressed()), this, SLOT (OpenButtonHandle())); //OPEN_DOOR BUTTON
    connect(ui->open_door_button, SIGNAL(released()), this, SLOT (ReleaseOpenButtonHandle())); //RELEASE OPEN_DOOR BUTTON
    connect(ui->close_door_button, SIGNAL(released()), this, SLOT (CloseButtonHandle())); //CLOSE_DOOR BUTTON
    connect(ui->power_out_button, SIGNAL(released()), this, SLOT (PowerOutHandle())); //POWER OUTAGE BUTTON
    connect(ui->fire_button, SIGNAL(released()), this, SLOT (FireHandle())); //FIRE BUTTON
    connect(ui->help_button, SIGNAL(released()), this, SLOT (HelpHandle()));
    connect(ui->blockage_button, SIGNAL(released()), this, SLOT (BlockageHandle()));

    //SPINNERS
    connect(ui->floor_spin, SIGNAL(valueChanged(int)), this, SLOT (FloorSpinHandle()));
    connect(ui->elevator_spin, SIGNAL(valueChanged(int)), this, SLOT (ElevatorSpinHandle()));
    connect(ui->elevator_weight_spin, SIGNAL(valueChanged(int)), this, SLOT (WeightSpinHandle()));


    //Console Log Initialization
    consoleString = new QString();
    consoleScroll = new QWidget(ui->scrollArea);
    consoleLayout = new QVBoxLayout(consoleScroll);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
-----------------------------------
 BUTTON HANDLES
-----------------------------------
*/
void MainWindow::UpButtonHandle(){
    int closest_elevator = cs->FindClosestElevator(ui->floor_spin->value(), 1);

    if (closest_elevator != -1){
        AddToLog(cs->fcps[ui->floor_spin->value()-1]->UpButtonPressed(), 2);
        AddToLog(cs->fcps[ui->floor_spin->value()-1]->IlluminateUpButton(), 2);
        AddToLog("[Floor Control Panel " + std::to_string(ui->floor_spin->value()) + "] Found closest elevator: ID#" + std::to_string(closest_elevator), 2);
        AddToLog("[Floor Control Panel " + std::to_string(ui->floor_spin->value()) + "] Elevator Called.", 2);

        MovementHandle(ui->floor_spin->value(), closest_elevator);
    } else {
        AddToLog("[Control System] All Elevators are Out of Order/Unavailable at this Moment.", 2);
    }
    AddToLog("--------------------------------------------",0);

}

void MainWindow::DownButtonHandle(){
    int closest_elevator = cs->FindClosestElevator(ui->floor_spin->value(), 0);

    if (closest_elevator != -1){
        AddToLog(cs->fcps[ui->floor_spin->value()-1]->DownButtonPressed(), 2);
        AddToLog(cs->fcps[ui->floor_spin->value()-1]->IlluminateDownButton(), 2);

        AddToLog("[Floor Control Panel " + std::to_string(ui->floor_spin->value()) + "] Found closest elevator: ID#" + std::to_string(closest_elevator), 2);
        AddToLog("[Floor Control Panel " + std::to_string(ui->floor_spin->value()) + "] Elevator Called.", 2);

        MovementHandle(ui->floor_spin->value(), closest_elevator);
    } else {
        AddToLog("[Control System] All Elevators are Out of Order/Unavailable at this Moment.", 2);
    }
    AddToLog("--------------------------------------------",0);
}

void MainWindow::OpenButtonHandle(){
    int elevatorID = ui->elevator_spin->value()-1;

    if (!cs->elevators[elevatorID]->InTransit()){ //Don't open the door during transit!
        cs->OpenDoors(elevatorID, "Button");
        AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Open Door Button was pressed, the Doors are Open", 1);
    }
    AddToLog("--------------------------------------------",0);
}

void MainWindow::ReleaseOpenButtonHandle(){
    int elevatorID = ui->elevator_spin->value()-1;

    if (!cs->elevators[elevatorID]->InTransit()){ //Don't open the door during transit!
        cs->CloseDoors(elevatorID);
        AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Open Door Button was released, the Doors are Closing", 1);
    }
    AddToLog("--------------------------------------------",0);
}

void MainWindow::CloseButtonHandle(){
    int elevatorID = ui->elevator_spin->value()-1;

    if (cs->elevators[elevatorID]->doors.IsDoorsOpen() == false){
        AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Close Door Button was Pressed, the Doors are Aready Closed, do Nothing", 1);
    } else {
        AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Close Door Button was Pressed, the Doors Close Quickly", 1);
        AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Ring Bell", 1);
    }

    cs->CloseDoors(elevatorID);
    AddToLog("--------------------------------------------",0);
}

void MainWindow::FloorButtonHandle(){

    QPushButton *button = qobject_cast<QPushButton*>(sender());
    int floor = button->property("floor").toInt() + 1;
    int elevatorID = ui->elevator_spin->value() - 1;

    MovementHandle(floor, elevatorID);
    AddToLog("--------------------------------------------",0);

}
/*
-----------------------------------
 SPIN HANDLES
-----------------------------------
*/
void MainWindow::FloorSpinHandle(){
    if (ui->floor_spin->value() == ui->num_floors_spin->value()){
        ui->up_button->setHidden(1);
        ui->down_button->setHidden(0);
    }
    else if (ui->floor_spin->value() == 1){
        ui->down_button->setHidden(1);
        ui->up_button->setHidden(0);
     }
    else {
        ui->up_button->setHidden(0);
        ui->down_button->setHidden(0);
    }
}

void MainWindow::ElevatorSpinHandle(){
    ui->level_indicator->display(cs->elevators[ui->elevator_spin->value()-1]->GetCurrentFloor()+1);
    ui->elevator_weight_spin->setValue(cs->elevators[ui->elevator_spin->value()-1]->GetCurrentWeight());
    if (cs->elevators[ui->elevator_spin->value()-1]->IsActive()){
        ui->elevator_floorButtonsFrame->setHidden(false);
        ui->elevator_otherButtonsFrame->setHidden(false);
        ui->elevator_externalButtons->setHidden(false);

    } else {
        ui->elevator_floorButtonsFrame->setHidden(true);
        ui->elevator_otherButtonsFrame->setHidden(true);
        ui->elevator_externalButtons->setHidden(true);

    }
}
/*
-----------------------------------
 ELEVATOR MOVEMENT HANDLES
-----------------------------------
*/
//Despite the name, this primairly focuses with UI Input and Output, all technicality relies on the control system still.
void MainWindow::MovementHandle(int desired_floor, int elevatorID){

    AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Elevator pending to depart from Floor "+std::to_string(cs->elevators[elevatorID]->GetCurrentFloor()+1), 0);
    std::string str = cs->ReadSignal("Pending", desired_floor, elevatorID);

    if (SafetyHandle(str, desired_floor, elevatorID)){
        /* Moving the Car Handle (If Elevator Travels more Than 1 Floor)
         ---------------------------------------------------------------------------*/
        if (str == "MoveCar"){
            str = cs->ReadSignal("CloseDoors", desired_floor, elevatorID);
            AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Closing Doors", 1); //If an elevator is called, always attempt to close the doors as a safety percaution.
            AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Ring Bell", 1);

            AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Elevator no Longer Pending, Departing.", 1);
            str = cs->ReadSignal("MoveCar", desired_floor, elevatorID);

            while (str == "MoveCar"){
                if (elevatorID+1 == ui->elevator_spin->value()){
                    ui->level_indicator->display(cs->elevators[elevatorID]->GetCurrentFloor()+1);
                }
                AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Elevator has Moved to Floor "+std::to_string(cs->elevators[elevatorID]->GetCurrentFloor() + 1), 1);
                str = cs->ReadSignal(str, desired_floor, elevatorID);
            }
            AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Elevator has Arrived at Floor "+std::to_string(cs->elevators[elevatorID]->GetCurrentFloor() + 1), 3);

            str = cs->ReadSignal("OpenDoors", desired_floor, elevatorID);
            AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Ring Bell", 1);
            AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Opening Doors [10s]", 1);
            AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Closing Doors", 1);
            AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Ring Bell", 1);

        /* Moving the Car Handle (If Elevator Is On Floor That was Called)
         ---------------------------------------------------------------------------*/
        } else if (str == "SameFloor"){
            AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Elevator is Staying Idle at Floor "+std::to_string(cs->elevators[elevatorID]->GetCurrentFloor() + 1), 1);

            str = cs->ReadSignal("OpenDoors", desired_floor, elevatorID);
            AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Ring Bell", 1);
            AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Opening Doors [10s]", 1);
            AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Closing Doors", 1);
            AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Ring Bell", 1);

        /* Moving the Car Handle (If the Signal goes straight to "StopCar", the Elevator has Traveled at MOST 1 Floor.)
         ---------------------------------------------------------------------------*/
        } else if (str == "StopCar"){
            str = cs->ReadSignal("CloseDoors", desired_floor, elevatorID);
            AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Closing Doors.", 1);
            AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Elevator no Longer Pending, Departing.", 1);
            AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Elevator has Arrived at Floor "+std::to_string(cs->elevators[elevatorID]->GetCurrentFloor() + 1), 3);

            str = cs->ReadSignal("OpenDoors", desired_floor, elevatorID);
            AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Ring Bell", 1);
            AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Opening Doors [10s]", 1);
            AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Closing Doors", 1);
            AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Ring Bell", 1);
        }
        if (elevatorID+1 == ui->elevator_spin->value()){
            ui->level_indicator->display(cs->elevators[elevatorID]->GetCurrentFloor()+1);
        }
    }
}

/*
-----------------------------------
 ELEVATOR SAFETY APPROACH HANDLES
-----------------------------------
*/
bool MainWindow::SafetyHandle(std::string signal, int desired_floor, int elevatorID){
    if (signal == "Overload"){
        cs->ReadSignal(signal, desired_floor, elevatorID);
        AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] is Overweight (" + std::to_string(cs->elevators[elevatorID]->GetCurrentWeight()) + "/15). All Potential Movement has been Stopped. ", 2);
        AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Overload Audio is Played", 2);
        AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Overload Display Show Message: 'The current elevator weight exceeds the capacity, please remove some weight and wait.'", 2);
        AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Doors Remain Open", 1);

    } else if (signal == "PowerOutage") {
        int safe_floor = building->GetSafeFloor();

        for (int i = 0; i < (int)cs->elevators.size(); ++i){
            std::string str = cs->ReadSignal(signal, safe_floor, i);
            AddToLog("[Control System] The Backup Battery is now in Use by Elevator ID#"+std::to_string(i), 2);
            AddToLog("[Elevator ID#"+std::to_string(i)+"] Elevator Departing to Safe Floor "+ std::to_string(safe_floor), 2);

            str = cs->ReadSignal(str, safe_floor, i);

            while (str == "MoveCar"){
                if (i+1 == ui->elevator_spin->value()){
                    ui->level_indicator->display(cs->elevators[i]->GetCurrentFloor());
                }
                AddToLog("[Elevator ID#"+std::to_string(i)+"] Elevator has Moved to Safe Floor "+std::to_string(cs->elevators[i]->GetCurrentFloor() + 1), 1);
                str = cs->ReadSignal(str, safe_floor, i);
            }
            AddToLog("[Elevator ID#"+std::to_string(i)+"] Elevator has Arrived at Safe Floor "+std::to_string(safe_floor), 3);
            cs->OpenDoors(elevatorID, "Button");
            AddToLog("[Elevator ID#"+std::to_string(i)+"] Doors are Remaining Open so Passengers can Exit ", 3);
        }
        AddToLog("--------------------------------------------",0);
        AddToLog("[Control System] Due to the Situation, all Elevators are now Currently Out of Order",2);
        AddToLog("--------------------------------------------",0);

        ui->elevator_floorButtonsFrame->setHidden(true);
        ui->elevator_otherButtonsFrame->setHidden(true);
        ui->elevator_externalButtons->setHidden(true);
        ui->upDownButtonsFrame->setHidden(true);

        return false;
    } else if (signal == "Fire"){
        int safe_floor = building->GetSafeFloor();

        for (int i = 0; i < (int)cs->elevators.size(); ++i){
            std::string str = cs->ReadSignal(signal, safe_floor, i);
            AddToLog("[Elevator ID#"+std::to_string(i)+"] Elevator Departing to Safe Floor "+ std::to_string(safe_floor), 2);

            str = cs->ReadSignal(str, safe_floor, i);

            while (str == "MoveCar"){
                if (i+1 == ui->elevator_spin->value()){
                    ui->level_indicator->display(cs->elevators[i]->GetCurrentFloor());
                }
                AddToLog("[Elevator ID#"+std::to_string(i)+"] Elevator has Moved to Safe Floor "+std::to_string(cs->elevators[i]->GetCurrentFloor() + 1), 1);
                str = cs->ReadSignal(str, safe_floor, i);
            }
            AddToLog("[Elevator ID#"+std::to_string(i)+"] Elevator has Arrived at Safe Floor "+std::to_string(safe_floor), 3);
             cs->OpenDoors(elevatorID, "Button");
             AddToLog("[Elevator ID#"+std::to_string(i)+"] Doors are Remaining Open so Passengers can Exit ", 3);
        }
        AddToLog("--------------------------------------------",0);
        AddToLog("[Control System] Due to the Situation, all Elevators are now Currently Out of Order",2);
        AddToLog("--------------------------------------------",0);
        ui->elevator_floorButtonsFrame->setHidden(true);
        ui->elevator_otherButtonsFrame->setHidden(true);
        ui->elevator_externalButtons->setHidden(true);
        ui->upDownButtonsFrame->setHidden(true);
        return false;

    } else if (signal == "Help"){
        cs->ReadSignal(signal, 0, elevatorID);
        ui->elevator_floorButtonsFrame->setHidden(true);
        ui->elevator_otherButtonsFrame->setHidden(true);
        ui->elevator_externalButtons->setHidden(true);

        AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Elevator Help Button has been pressed. ", 2);
        AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Elevator has been Stopped. ", 2);
        AddToLog("[Control System] Control System has dialed the Building's Safety Services",2);
        AddToLog("[Control System] Control System has connected the call to Elevator ID#"+std::to_string(elevatorID)+" audio system.",2);
        AddToLog("[Control System] All Actions with Elevator ID#"+std::to_string(elevatorID)+" have been Paused, the Elevator is now Currently Out of Order",2);
        AddToLog("--------------------------------------------",0);

    } else if (signal == "Blockage"){
        cs->ReadSignal(signal, desired_floor, elevatorID);
        AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] The Elevator is Detecting Door Blockage. All Potential Movement has been Stopped. ", 2);
        AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] [After 10 Seconds] Blockage Audio is Played", 2);
        AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] [After 10 Seconds] Blockage Display Show Message: 'The doors are blocked, please move away all hazards from the doors.'", 2);
        AddToLog("[Elevator ID#"+std::to_string(elevatorID)+"] Doors Remain Open", 1);
    }
    return true;
}

/*
-----------------------------------
 SAFETY INPUT HANDLES
-----------------------------------
*/
void MainWindow::WeightSpinHandle(){
    cs->elevators[ui->elevator_spin->value()-1]->SetCurrentWeight(ui->elevator_weight_spin->value());
}

void MainWindow::PowerOutHandle(){
    SafetyHandle("PowerOutage", 0, 0);
}

void MainWindow::FireHandle(){
    SafetyHandle(building->FireAlarmPressed(), 0, 0);
}

void MainWindow::HelpHandle(){
    SafetyHandle("Help",0,ui->elevator_spin->value()-1);
}

void MainWindow::BlockageHandle(){
    if (cs->elevators[ui->elevator_spin->value()-1]->doors.sensors.DetectBlockage() == false){
        cs->elevators[ui->elevator_spin->value()-1]->doors.sensors.SetBlockage(true);
        AddToLog("[Elevator ID#"+std::to_string(ui->elevator_spin->value()-1)+"] There WILL now be Blockage interacting with the Elevator.", 1);
        AddToLog("--------------------------------------------",0);

    } else {
        cs->elevators[ui->elevator_spin->value()-1]->doors.sensors.SetBlockage(false);
        AddToLog("[Elevator ID#"+std::to_string(ui->elevator_spin->value()-1)+"] There WILL NOT now be Blockage interacting with the Elevator.", 1);
        AddToLog("--------------------------------------------",0);

    }
}
/*
-----------------------------------
 INITIALIZATION
-----------------------------------
*/
void MainWindow::StartSimulation(){
    int numOfFloors = ui->num_floors_spin->value();
    int numOfCars = ui->num_cars_spin->value();
    int safeFloor = ui->safe_floor_spin->value();

    if (numOfCars == 0 || numOfFloors == 0){
        AddToLog("[ERROR] Number of Floors/Number of Cars CANNOT be 0.", 2);
    } else if (safeFloor > numOfFloors){
        AddToLog("[ERROR] The designated safe floor CANNOT be greater than the number of floors.", 2);
    } else if (numOfFloors <= 1) {
        AddToLog("[ERROR] There must be >1 floors.", 2);
    } else {
        AddToLog("[NOTICE] Simulation has started.", 2);
        AddToLog("--------------------------------------------",0);

        //HIDE/UNHIDE ESSENTIAL BUTTONS
        ui->upDownButtonsFrame->setHidden(0);
        ui->elevator_informationFrame->setHidden(0);
        ui->elevator_floorButtonsFrame->setHidden(0);
        ui->elevator_otherButtonsFrame->setHidden(0);
        ui->elevator_externalButtons->setHidden(0);
        ui->start_button->setHidden(1);

        //DISABLE THE BEGINNING SETTINGS
        ui->num_floors_spin->setDisabled(1);
        ui->num_cars_spin->setDisabled(1);
        ui->safe_floor_spin->setDisabled(1);

        //SET THE MINIMUM AND MAXIMUM OF THE FLOOR SPINNER
        ui->floor_spin->setMinimum(1);
        ui->floor_spin->setMaximum(numOfFloors);

        //DYNAMICALLY ADDING FLOOR BUTTONS
        //QLayout* layout = ui->elevator_floorButtonsFrame->layout();
        //layout = new QVBoxLayout(ui->elevator_floorButtonsFrame);

        QGridLayout* layout = ui->gridLayout;
        //layout = new QVBoxLayout(ui->elevator_floorButtonsFrame);

        int row = 0;
        int col = 0;

        for (int i = 0; i < numOfFloors; ++i){
            QPushButton *button = new QPushButton(QString::number(i+1), this);
            button->setProperty("floor",i);
            connect(button, SIGNAL(released()), this, SLOT (FloorButtonHandle()));
            layout->addWidget(button,row,col,4,1);
            col+=1;
            if(col == 3){
                row += 1;
                col = 0;
            }
        }

        //SET ELEVATOR SPINNER MAXIMUM
        ui->elevator_spin->setMaximum(numOfCars);

        cs = new Control_System(numOfCars, numOfFloors);
        building = new Building(safeFloor);
        building->cs = cs;
    }
}

void MainWindow::AddToLog(std::string str, int color){
    /*
     Let 0 represent Black
     Let 1 represent Blue
     Let 2 Represent Red
     Let 3 Represent Green
     */
    QLabel *label = new QLabel(QString(QString::fromStdString(str)), consoleScroll);
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    if (color == 0){
        label->setStyleSheet("QLabel {color : black;}");
    } else if (color == 1){
        label->setStyleSheet("QLabel {color : blue;}");
    } else if (color == 2){
        label->setStyleSheet("QLabel {color : red;}");
    } else if (color == 3){
        label->setStyleSheet("QLabel {color : green;}");
    }
    consoleLayout->addWidget(label);
    ui->scrollArea->setWidget(consoleScroll);
}
