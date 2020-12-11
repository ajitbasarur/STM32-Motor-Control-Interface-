#ifndef __MC_INTERFACE_H__
#define __MC_INTERFACE_H__

#include<iostream>
// Used for including fixed integer types
#include<cstdint>

//Frame communication protocol header file
#include"frame_communication_protocol.h"

using namespace std;

class frame_class {
public:
	// Constructor
	frame_class();
	
	// This constuctor is used when creating a send frame
	frame_class(uint8_t header, uint8_t ui8length, uint8_t *pui8Payload) {
		ui8FrameHeader = header; 
		ui8PayloadLength = ui8length;
		ui8Payload = new uint8_t[ui8length];
		// Copy the payload
		memcpy(ui8Payload, pui8Payload, ui8PayloadLength);
		// Compute the checksum
		compute_checksum();
		ui8FrameSize = 3 + ui8PayloadLength;
	}

	// Destructor
	~frame_class(){
		delete [] ui8Payload;
	}

	// Return the checksum
	uint8_t get_checksum() {
		return ui8Checksum;
	}

	// Return the frame
	uint8_t* get_frame() {
		// Length of buffer
		// header + payloadlength + payload + crc
		uint8_t pui8FrameBuff = new uint8_t[ui8FrameSize];
		// Copy header
		pui8FrameBuff[0] = ui8FrameHeader;
		// Copy payload length
		pui8FrameBuff[1] = ui8PayloadLength;
		// Copy the payload
		memcpy(&pui8FrameBuff[2], ui8Payload, ui8PayloadLength);
		// Copy the checksum
		pui8FrameBuff[ui8FrameSize-1] = ui8Checksum;

		return pui8FrameBuff;
	}

	uint8_t get_frame_size() {
		return ui8FrameSize;
	}

private:
	// Compute Checksum
	compute_checksum() {
		// Compute the accumulator
		uint16_t ui16Accumulator = ui8FrameHeader + ui8PayloadLength;
		
		// Compute the sum of all payloads
		if (NULL != ui8Payload){
			for(i=0;i<ui8PayloadLength;i++) {
				ui16Accumulator += (pui8Payload[i] & 0xFF);
			}
		}
		
		// Low-byte
		ui8CheckSum = ui16Accumulator & 0xFF;
		// High-byte
		ui8CheckSum += (ui16Accumulator>>8 & 0xFF);
	}

protected:
	uint8_t ui8FrameHeader;
	uint8_t ui8PayloadLength;
	uint8_t ui8Payload[];
	uint8_t ui8Checksum;
	uint8_t ui8FrameSize;
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
	
	private:
	// Set register
	uint8_t set_reg(uint8_t ui8Reg, uint8_t ui8RegSize, int8_t *pi8RegVal);

	// Get register
	uint8_t get_reg(uint8_t ui8Reg, uint8_t ui8RegSize, int8_t *i8RegVal);

	uint8_t execute_command(uint8_t ui8Command);

	uint8_t get_board_info(uint8_t *pui8Buffer, uint8_t ui8BuffSize);

	uint8_t set_ramp_speed(int32_t i32FinalSpeed, uint16_t ui16DurationMs);

	uint8_t set_current_reference(int16_t i16IqRef, int16_t i16IdRef);

	uint8_t get_firmware_version(uint8_t *pui8Buffer, uint8_t ui8BuffSize);		


}

#endif // __MC_INTERFACE_H__