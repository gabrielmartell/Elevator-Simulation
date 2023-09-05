Name: Gabriel Martell
Stu.ID: 101191857

Files:
Headers:
	audio_system.h
	building.h
	control_system.h
	defs.h
	display_system.h
	elevator_car.h
	elevator_doors.h
	elevator_doors_sensors.h
	floor_control_panel.h
	floor_sensor.h
	mainwindow.h
	
CPP:
	audio_system.cpp
	building.cpp
	control_system.cpp
	defs.cpp
	display_system.cpp
	elevator_car.cpp
	elevator_doors.cpp
	elevator_doors_sensors.cpp
	floor_control_panel.cpp
	floor_sensor.cpp
	mainwindow.cpp
	main.cpp
	
QT Forms:
	mainwindow.ui
	
Instructions:
	To run the simulation, press the START button with provided elevator information.
	You will then be presented to 4 panels that you can interact with.
	
	The Car Control Panel:
		Change what elevator you are controlling,
		Change the weight of said elevator, measured in Tonnes.
		Pick the floor of said elevator.
		Open the doors of said elevator.
		Close the doors of said elevator.
		Press the Help button of said elevator.
	The Floor Control Panel:
		Pick the floor you are on.
		Pick to go up.
		Pick to go down.
	External Signal Panel:
		Call the Fire Alarm
		Call a Power Outage
		Call a Blockage on a given Elevator predetermined in the Car Control Panel
	Info Panel:
		Holds Information pertinent to the simulation, the ones you determined before you pressed
		the start button.

	All of these panels go hand-in-hand to present a elevator simulation.
	
