#ifndef __MC_INTERFACE_H__
#define __MC_INTERFACE_H__

#include<iostream>
// Used for including fixed integer types
#include<cstdint>

//Frame communication protocol header file
#include"frame_communication_protocol.h"
#include "serial_port.hpp"

using namespace std;

class frame_class {
public:
	
	// This constuctor is used when creating a send frame
	frame_class(uint8_t header, uint8_t ui8length, uint8_t *pui8Payload) {
		ui8FrameHeader = header; 
		ui8PayloadLength = ui8length;
		this->pui8Payload = new uint8_t[ui8length];
		// Copy the payload
		cout << "Copying the data \n";
		memcpy(this->pui8Payload, pui8Payload, ui8PayloadLength);
		// Compute the checksum
		cout << "Computing the checksum \n";
		compute_checksum();
		ui8FrameSize = 3 + ui8PayloadLength;
	}

	// Destructor
	~frame_class(){
		delete [] pui8Payload;
	}

	// Return the checksum
	uint8_t get_checksum() {
		return ui8CheckSum;
	}

	// Return the frame
	uint8_t* get_frame() {
		// Length of buffer
		// header + payloadlength + payload + crc
		uint8_t* pui8FrameBuff = new uint8_t[ui8FrameSize];
		// Copy header
		pui8FrameBuff[0] = ui8FrameHeader;
		// Copy payload length
		pui8FrameBuff[1] = ui8PayloadLength;
		// Copy the payload
		memcpy(&pui8FrameBuff[2], pui8Payload, ui8PayloadLength);
		// Copy the checksum
		pui8FrameBuff[ui8FrameSize-1] = ui8CheckSum;

		cout << pui8FrameBuff;
		return pui8FrameBuff;
	}

	uint8_t get_frame_size() {
		return ui8FrameSize;
	}

private:
	// Compute Checksum
	void compute_checksum(void) {
		// ucounter
		uint8_t i = 0;
		// Compute the accumulator
		uint16_t ui16Accumulator = ui8FrameHeader + ui8PayloadLength;
		
		// Compute the sum of all payloads
		if (NULL != pui8Payload){
			for(i=0;i<ui8PayloadLength;i++) {
				ui16Accumulator += (pui8Payload[i] & 0xFF);
			}
		}
		
		// Low-byte
		ui8CheckSum = ui16Accumulator & 0xFF;
		// High-byte
		ui8CheckSum += (ui16Accumulator>>8 & 0xFF);
	}

public:
	uint8_t ui8FrameHeader;
	uint8_t ui8PayloadLength;
	uint8_t ui8CheckSum;
	uint8_t ui8FrameSize;
	uint8_t* pui8Payload;
};

class mc_interface_class {
	// Define variables
public:

	// Define functions
public:
	// Constructor
	mc_interface_class();
	// Destructor
	~mc_interface_class();

	// Define APIs for the applications
	// Set register APIs
	bool set_speed_Kp(int16_t i16SpeedVal);

	// Get register APIs
	int16_t get_speed_Kp();
	
	// Initialize the serial port interface
	int8_t init(const char* device);
private:	
	// Set register
	int8_t set_reg(uint8_t ui8Reg, uint8_t ui8RegSize, int8_t *pi8RegVal);

	// Get register
	uint8_t get_reg(uint8_t ui8Reg, uint8_t ui8RegSize, int8_t *i8RegVal);

	uint8_t execute_command(uint8_t ui8Command);

	uint8_t get_board_info(uint8_t *pui8Buffer, uint8_t ui8BuffSize);

	uint8_t set_ramp_speed(int32_t i32FinalSpeed, uint16_t ui16DurationMs);

	uint8_t set_current_reference(int16_t i16IqRef, int16_t i16IdRef);

	uint8_t get_firmware_version(uint8_t *pui8Buffer, uint8_t ui8BuffSize);		

	// Private variables declaration
private:
	class serial_port spClass;
};

#endif // __MC_INTERFACE_H__