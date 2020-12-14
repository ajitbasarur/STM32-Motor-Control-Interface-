#include <cstdio>
#include <iostream>
#include <cstdint>

#include "mc_interface.hpp"

using namespace std;


int main(){
	class mc_interface_class mcInterface;

	cout << "Starting the application \n";
	//mcInterface.init("/dev/cu.usbmodem14203");
	mcInterface.init("/dev/ttyACM0");
	cout << "Initialization done \n";
#if 0	
	if(mcInterface.set_speed_Kp(1510) < 0) {
		cout << "set_speed_Kp() failed \n";
	}
	int16_t i16KpValue;
	i16KpValue = mcInterface.get_speed_Kp();
	cout << "Kp value is " << i16KpValue << endl;
#endif

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
	// Set the control mode to speed
	cout << "Setting the control mode to speed\n";
	if(mcInterface.set_control_mode(0) < 0) {
		cout << "set_control_mode() failed \n";
	}	
	
	sleep(0.1);
	// Setting the speed
	int32_t i32SpeedVal = 4000;
	cout << "Setting the speed to " << i32SpeedVal << " RPM\n" ;
	if(mcInterface.set_speed_ramp(i32SpeedVal) < 0) {
		cout << "set_speed_ramp() failed \n";
	}	
	
	// Starting the motor
	cout << "Starting the motor\n";
	mcInterface.start_motor();
	sleep(1);
	cout << "Stopping the motor\n";
	mcInterface.stop_motor();
	cout << "Ending the application \n";

	return 0;
};