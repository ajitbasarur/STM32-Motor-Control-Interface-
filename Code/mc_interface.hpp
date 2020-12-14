#ifndef __MC_INTERFACE_H__
#define __MC_INTERFACE_H__

#include<iostream>
// Used for including fixed integer types
#include<cstdint>
#include<cstring>
//Frame communication protocol header file
#include"frame_communication_protocol.h"
#include "serial_port.hpp"

using namespace std;

class frame_class {
public:
	
	// This constuctor is used when creating a send frame
	frame_class(uint8_t header, uint8_t ui8Reg, uint8_t ui8length, uint8_t *pui8Payload) {
		ui8FrameHeader = header; 
		ui8PayloadLength = ui8length+1;
		this->pui8Payload = new uint8_t[ui8PayloadLength];
		// Copy the payload
		this->pui8Payload[0] = ui8Reg;
		if((NULL != pui8Payload) || (0!= ui8length)) {
			std::memcpy(&this->pui8Payload[1], pui8Payload, ui8length);
		}
		// Compute the checksum
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
		std::memcpy(&pui8FrameBuff[2], pui8Payload, ui8PayloadLength);
		// Copy the checksum
		pui8FrameBuff[ui8FrameSize-1] = ui8CheckSum;

		//cout << pui8FrameBuff;	
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
	int8_t set_speed_Kp(int16_t i16SpeedVal);
	int8_t set_speed_ramp(int32_t i32SpeedRampVal) ;
	int8_t set_control_mode(int8_t i8ControlMode);
	int8_t set_torque_ref(int16_t i16TorqueRefVal);
	int8_t set_flux_ref(int16_t i16FluxRefVal);

	// Get register APIs
	int16_t get_speed_Kp();
	uint16_t get_heat_sink_temp(void) ;
	uint16_t get_bus_voltage(void) ;
	
	// Execute command APIs
	int8_t start_stop_motor(void);
	int8_t encoder_align(void);
	int8_t fault_ack(void);
	int8_t start_motor(void);
	int8_t stop_motor(void);
	int8_t stop_ramp(void);
	
	// Initialize the serial port interface
	int8_t init(const char* device);
private:	
	// Set register
	int8_t set_reg(uint8_t ui8Reg, uint8_t ui8RegSize, int8_t *pi8RegVal);

	// Get register
	int8_t get_reg(uint8_t ui8Reg, uint8_t ui8RegSize, int8_t *i8RegVal);

	int8_t execute_command(uint8_t ui8Command);

	uint8_t get_board_info(uint8_t *pui8Buffer, uint8_t ui8BuffSize);

	uint8_t set_ramp_speed(int32_t i32FinalSpeed, uint16_t ui16DurationMs);

	uint8_t set_current_reference(int16_t i16IqRef, int16_t i16IdRef);

	uint8_t get_firmware_version(uint8_t *pui8Buffer, uint8_t ui8BuffSize);		

private:
	// Private function declarations
	// Compute Checksum
	int8_t validate_checksum(uint8_t *pui8Buffer, uint8_t ui8BuffSize, uint8_t ui8Crc) {
		// ucounter
		uint8_t i = 0;
		uint8_t ui8CheckSum = 0;
		// Compute the accumulator
		uint16_t ui16Accumulator = 0;
		
		// Compute the sum of all payloads
		if (NULL != pui8Buffer){
			for(i=0;i<ui8BuffSize;i++) {
				ui16Accumulator += (pui8Buffer[i] & 0xFF);
			}
		}
		
		// Low-byte
		ui8CheckSum = ui16Accumulator & 0xFF;
		// High-byte
		ui8CheckSum += (ui16Accumulator>>8 & 0xFF);
		
		if (ui8CheckSum == ui8Crc) {
			return 0;
		} else {
			return -1;
		}
	}	
	// Private variables declaration
private:
	class serial_port spClass;
};

#endif // __MC_INTERFACE_H__
