// It defines all the functions of the class mc_interface

#include<iostream>
#include"mc_interface.hpp"

using namespace std;

void mc_interface_class::mc_interface_class() {
	cout << "mc_interface_class constructor()\n";
}

void mc_interface_class::~mc_interface_class() {
	cout << "mc_interface_class destructor()\n";
}

// Set the 8-bit register
uint8_t mc_interface_class::set_reg(uint8_t ui8Reg, uint8_t ui8RegSize, int8_t *pi8RegVal) {
	// Create header
	uint8_t ui8Header = FRAME_HEADER(MOTOR_NUMBER, FRAME_CODE_SET_REG);
	
	// Create the frame message
	friend class frame_class cSetRegFrame(ui8Header, ui8RegSize, pi8RegVal);
	// Get the frame 
	uint8_t *pui8Buff = cSetRegFrame.get_frame();
}

bool mc_interface_class::set_speed_Kp(int16_t i16SpeedVal) {
	set_reg(REG_ID_SPEED_KP, sizeof(REG_TYPE_SPEED_KP), i16SpeedVal);
	// Read back the response
}

uint8_t  mc_interface_class::get_firmware_version(uint8_t *pui8Buffer, uint8_t ui8BuffSize) {
	// Create header
	uint8_t ui8Header = FRAME_HEADER(MOTOR_NUMBER, FRAME_CODE_GET_FW_VERSION);
		// Compute the checksum
		uint8_t ui8CheckSum = compute_checksum(ui8Header, NULL, 0);
	
}