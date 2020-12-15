#include <cstdio>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <sstream>
#include "mc_interface.hpp"

using namespace std;

#define COMMAND_LINE_PARSE

#ifdef COMMAND_LINE_PARSE
void command_parse_help(){
	cout << "Speed control mode --> ./test_motor_interface.o mode 0 \n";
	cout << "Torque control mode --> ./test_motor_interface.o mode 1 \n";
	cout << "Set the speed  to 2000 RPM --> ./test_motor_interface.o speed 2000 \n";
	cout << "Set the speed  to -3000 RPM --> ./test_motor_interface.o speed -3000 \n";
	cout << "Set the torque/current  to 12000 mA --> ./test_motor_interface.o torque 12000 \n";
	cout << "Set the torque/current  to -5000 mA --> ./test_motor_interface.o torque -5000 \n";
	cout << "Start the encoder alignment --> ./test_motor_interface.o align \n";
	cout << "Start the motor --> ./test_motor_interface.o start \n";
	cout << "Stop the motor --> ./test_motor_interface.o stop \n";
	cout << "Toggle the start stop  --> ./test_motor_interface.o toggle_start_stop \n";
};
#endif 


int main(int argc, char * argv[]){
	class mc_interface_class mcInterface;

	cout << "Starting the application \n";

	//mcInterface.init("/dev/cu.usbmodem14203");
	if(mcInterface.init("/dev/ttyACM0") < 0) {
		cout << "mc_init() failed\n";
		return -1;
	}
	cout << "Initialization done. \n";
	
#ifdef COMMAND_LINE_PARSE
	int i = 0;
	cout << "Printing all the arguments \n";
	for(i=0; i< argc; i++) {
		cout << argv[i] << "\t";
	}
	cout << "\n";

		
	if (argc < 2) {
		cout <<" No valid number of input arguments. Exiting the program\n";
		return -1;
	}
	string command(argv[1]);
	
	if(0 == command.compare("mode")) {
		stringstream ssValue(argv[2]);
		int16_t i16Val = 0;
		ssValue >> i16Val;
		if(0 == i16Val) {
				cout << "Setting to speed mode\n";
		} else if(1 == i16Val) {
			cout << "Setting to torque mode\n";
		} else {
			cout << "Invalid mode " << i16Val << "\n";
		}
		if(mcInterface.set_control_mode(i16Val) < 0) {
			cout << "set_control_mode() failed \n";
			}	
	} else if (0 == command.compare("speed")) {
		stringstream ssValue(argv[2]);
		int32_t i32Val = 0;
		ssValue >> i32Val;
		cout << "Setting speed to " << i32Val  <<" RPM\n";
		if(mcInterface.set_speed_ramp(i32Val) < 0) {
			cout << "set_speed_ramp() failed \n";
		}			
	}	else if (0 == command.compare("torque")) {
		stringstream ssValue(argv[2]);
		int16_t i16Val = 0;
		ssValue >> i16Val;
		cout << "Setting torque to " << i16Val  <<" mA\n";
		if(mcInterface.set_torque_ref(i16Val) < 0) {
			cout << "set_torque_ref() failed \n";
		}			
	}	else if (0 == command.compare("toggle_start_stop")) {
		cout << "Toggling start stop \n";
		if(mcInterface.start_stop_motor() < 0) {
			cout << "start_stop_motor() failed \n";
			}
	} else if (0 == command.compare("start")) {
		cout << "Starting the motor\n";
		if(mcInterface.start_motor() < 0) {
			cout << "start_motor() failed \n";
			}		
	} else if (0 == command.compare("stop")) {
		cout << "Stopping the motor\n";
		if(mcInterface.stop_motor() < 0) {
			cout << "stop_motor() failed \n";
			}				
	} else if (0 == command.compare("align")) {
		cout << "Encoder alignment\n";
		if(mcInterface.encoder_align() < 0) {
			cout << "encoder_align() failed \n";
			}				
	}	else {
		cout << "Invalid argument. Provide on of the following arguments\n";
		command_parse_help();
		
	}
	cout << "Ending the application \n";

#else	

	//mcInterface.init("/dev/cu.usbmodem14203");
	mcInterface.init("/dev/ttyACM0");
	cout << "Initialization done. \n";

	if(mcInterface.set_speed_Kp(1510) < 0) {
		cout << "set_speed_Kp() failed \n";
	}
	int16_t i16KpValue;
	i16KpValue = mcInterface.get_speed_Kp();
	cout << "Kp value is " << i16KpValue << endl;

	// Reading the heat sink temperature
	uint16_t get_bus_voltage;
	get_bus_voltage = mcInterface.get_heat_sink_temp();
	cout << "Heat Sink Temperatue is " << get_bus_voltage << endl;
	
	// Reading the bus voltage
	uint16_t ui16BusVoltage;
	ui16BusVoltage = mcInterface.get_bus_voltage();
	cout << "Bus Voltage is " << ui16BusVoltage << endl;	
	
	// Align the encoder
	cout << "Starting the encoder alignment\n";
	mcInterface.encoder_align();
	sleep(1);
	cout << "Encoder alignment is over\n";

	// Starting the motor
	mcInterface.fault_ack();
	
	cout << "Starting the motor\n";
	mcInterface.stop_motor();
	
	sleep(1);
	
#if 0
	// Set the control mode to speed
	cout << "Setting the control mode to speed\n";
	if(mcInterface.set_control_mode(0) < 0) {
		cout << "set_control_mode() failed \n";
	}	
	// Setting the speed
	int32_t i32SpeedVal = 1000;
	cout << "Setting the speed to " << i32SpeedVal << " RPM\n" ;
	if(mcInterface.set_speed_ramp(i32SpeedVal) < 0) {
		cout << "set_speed_ramp() failed \n";
	}	
#else
	// Set the control mode to current
	cout << "Setting the control mode to current\n";
	if(mcInterface.set_control_mode(1) < 0) {
		cout << "set_control_mode() failed \n";
	}	
	// Setting the speed
	int16_t i16TorqueRefVal = 0x1500;
	cout << "Setting the current to " << i16TorqueRefVal << " mA\n" ;
	if(mcInterface.set_torque_ref(i16TorqueRefVal) < 0) {
		cout << "set_torque_ref() failed \n";
	}		
#endif	

	mcInterface.start_stop_motor();
	sleep(5);
	cout << "Stopping the motor\n";
	mcInterface.start_stop_motor();
	cout << "Ending the application \n";
#endif // COMMAND_LINE_PARSE
	return 0;
};