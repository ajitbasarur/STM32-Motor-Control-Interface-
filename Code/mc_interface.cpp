// It defines all the functions of the class mc_interface

#include<iostream>
#include<mc_interface>

void mc_interface_class::mc_interface_class() {
	cout << "mc_interface_class constructor()\n";
}

void mc_interface_class::~mc_interface_class() {
	cout << "mc_interface_class destructor()\n";
}

uint8_t mc_interface_class::compute_checksum(uint8_t ui8Header, uint8_t *pui8Payload, uint8_t ui8Len`) {
	// Compute the accumulator
	uint16_t ui16Accumulator = ui8Header + ui8Len;
	uint8_t ui8CheckSum = 0;

	for(i=0;i<ui8Len;i++) {
		ui16Accumulator += (pui8Payload[i] & 0xFF);
	}
	
	// Low-byte
	ui8CheckSum = (ui16Accumulator & 0xFF);
	// High-byte
	ui8CheckSum += (ui16Accumulator>>8 & 0xFF);

	return ui8CheckSum;
}

// Set the 8-bit register
uint8_t mc_interface_class::set_reg(uint8_t ui8Reg, int8_t i8RegVal) {
	// Create header
	uint8_t ui8Header = FRAME_HEADER(MOTOR_NUMBER, FRAME_CODE_SET_REG);
	// Set the length
	uint8_t ui8PayloadLength = sizeof(i8RegVal);
	// Compute the checksum
	uint8_t ui8CheckSum = compute_checksum(ui8Header, &i8RegVal, ui8PayloadLength);

	// Format a message
	// Frame memory size
	uint8_t ui8FrameSize = sizeof(ui8Header) + sizeof(ui8PayloadLength);

	ui8FrameSize += sizeof(i8RegVal) + sizeof(ui8CheckSum);

	uint8_t *ui8Frame = new uint8_t(ui8FrameSize);

}