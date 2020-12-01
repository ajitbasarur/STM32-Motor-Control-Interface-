#ifndef __MC_INTERFACE_H__
#define __MC_INTERFACE_H__

#include<iostream>
// Used for including fixed integer types
#include<cstdint>

using namespace std;

class mc_interface_class {
	// Define variables
	public:

	// Define functions
	public:
		// Constructor
		mc_interface_class();
		// Destructor
		~mc_interface_class();
		// Define overloaded function
		// Set register
		uint8_t set_reg(uint8_t ui8Reg, int8_t i8RegVal);
		uint8_t set_reg(uint8_t ui8Reg, uint8_t ui8RegVal);
		uint8_t set_reg(uint8_t ui8Reg, int16_t i16RegVal);
		uint8_t set_reg(uint8_t ui8Reg, uint16_t ui16RegVal);
		uint8_t set_reg(uint8_t ui8Reg, int32_t i32RegVal);
		uint8_t set_reg(uint8_t ui8Reg, uint32_t ui32RegVal);

		// Get register
		uint8_t get_reg(uint8_t ui8Reg, int8_t *i8RegVal);
		uint8_t get_reg(uint8_t ui8Reg, uint8_t *ui8RegVal);
		uint8_t get_reg(uint8_t ui8Reg, int16_t *i16RegVal);
		uint8_t get_reg(uint8_t ui8Reg, uint16_t *ui16RegVal);
		uint8_t get_reg(uint8_t ui8Reg, int32_t *i32RegVal);
		uint8_t get_reg(uint8_t ui8Reg, uint32_t *ui32RegVal);

		uint8_t execute_command(uint8_t ui8Command);

		uint8_t get_board_info(uint8_t *pui8Buffer, uint8_t ui8BuffSize);

		uint8_t set_ramp_speed(int32_t i32FinalSpeed, uint16_t ui16DurationMs);

		uint8_t set_current_reference(int16_t i16IqRef, int16_t i16IdRef);

		uint8_t get_firmware_version(uint8_t *pui8Buffer, uint8_t ui8BuffSize);
	
	private:
		struct frame {
			uint8_t ui8FrameHeader;
			uint8_t ui8PayloadLength;
			uint8_t ui8Checksum;
			uint8_t ui8Payload[];
			frame(uint8_t length=1, uint8_t header, uint8_t checksum) {
				: ui8PayloadLength(ui8length),
				  ui8Payload(new uint8_t[ui8length]),
				  ui8FrameHeader(header),
				  ui8Checksum(checksum);
			}
			~frame(){
				delete [] ui8Payload;
			}
		};


}

#endif // __MC_INTERFACE_H__