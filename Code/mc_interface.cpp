// It defines all the functions of the class mc_interface

#include<iostream>
#include"mc_interface.hpp"


using namespace std;

 mc_interface_class::mc_interface_class() {
	cout << "mc_interface_class constructor()\n";
}

mc_interface_class::~mc_interface_class() {
	cout << "mc_interface_class destructor()\n";
}

int8_t mc_interface_class::init(const char* device) {
	// Serial port is always configured for the fixed baud rate.
	return spClass.port_open(device, 115200);
}

// Set the 8-bit register
int8_t mc_interface_class::set_reg(uint8_t ui8Reg, uint8_t ui8RegSize, int8_t *pi8RegVal) {
	// Create header
	uint8_t ui8Header = FRAME_HEADER(MOTOR_NUMBER, FRAME_CODE_SET_REG);
	
	// Create the frame message
	class frame_class cSetRegFrame(ui8Header, ui8Reg, ui8RegSize, (uint8_t *)pi8RegVal);
	// Get the frame 
	uint8_t *pui8Buff = cSetRegFrame.get_frame();
	uint8_t ui8BuffSize = cSetRegFrame.get_frame_size();

	int8_t i8Return = spClass.port_write(pui8Buff, ui8BuffSize);
	if(i8Return < 0) {
		cout << "set_reg() failed\n";
		return -1;
	}
	// Read the data
	uint8_t* pui8ReadBuff = new uint8_t[5];
	i8Return = spClass.port_read(pui8ReadBuff, 4);
	if(i8Return < 0) {
		cout << "reg_read() failed\n";
		return -1;
	} 
	
	return 0;
}

bool mc_interface_class::set_speed_Kp(int16_t i16SpeedVal) {
	set_reg(REG_ID_SPEED_KP, sizeof(REG_TYPE_SPEED_KP), (int8_t *)&i16SpeedVal);
	// Read back the response
	return true;
}

uint8_t  mc_interface_class::get_firmware_version(uint8_t *pui8Buffer, uint8_t ui8BuffSize) {
	// Create header
	//uint8_t ui8Header = FRAME_HEADER(MOTOR_NUMBER, FRAME_CODE_GET_FW_VERSION);
	// Compute the checksum
	//uint8_t ui8CheckSum = compute_checksum();
	return 0;
	
}