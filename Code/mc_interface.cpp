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

// Execute the command
int8_t mc_interface_class::execute_command(uint8_t ui8Command) {
	// Create header
	uint8_t ui8Header = FRAME_HEADER(MOTOR_NUMBER, FRAME_CODE_EXECUTE_CMD);
	
	// Create the frame message
	class frame_class cExecCommandFrame(ui8Header, ui8Command, 0, NULL);
	// Get the frame 
	uint8_t *pui8Buff = cExecCommandFrame.get_frame();
	uint8_t ui8BuffSize = cExecCommandFrame.get_frame_size();

	int8_t i8Return = spClass.port_write(pui8Buff, ui8BuffSize);
	if(i8Return < 0) {
		cout << "serial_port_write() failed\n";
		return -1;
	}
	// Read the data
	uint8_t ui8ResponseSize = 3;
	uint8_t* pui8ReadBuff = new uint8_t[ui8ResponseSize];
	i8Return = spClass.port_read(pui8ReadBuff, 1);
	if(i8Return < 0) {
		cout << "serial_port_read() failed\n";
		return -1;
	} 
	
	if(0xFF == pui8ReadBuff[0]) {
		cout << "NACK received\n";
		spClass.flush();
		return -1;
	} else {
		i8Return = spClass.port_read(&pui8ReadBuff[1], 2);
		if(i8Return < 0) {
			cout << "serial_port_read() failed\n";
			return -1;
		}	
		return 0;
	}
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
		cout << "serial_port_write() failed\n";
		return -1;
	}
	// Read the data
	uint8_t ui8ResponseSize = 3;
	uint8_t* pui8ReadBuff = new uint8_t[ui8ResponseSize];
	i8Return = spClass.port_read(pui8ReadBuff, 1);
	if(i8Return < 0) {
		cout << "serial_port_read() failed\n";
		return -1;
	} 
	
	if(0xFF == pui8ReadBuff[0]) {
		cout << "NACK received\n";
		spClass.flush();
		return -1;
	} else {
		i8Return = spClass.port_read(&pui8ReadBuff[1], 2);
		if(i8Return < 0) {
			cout << "reg_read() failed\n";
			return -1;
		}	
		return 0;
	}
}

// Set the 8-bit register
int8_t mc_interface_class::get_reg(uint8_t ui8Reg, uint8_t ui8RegSize, int8_t *i8RegVal){
	// Create header
	uint8_t ui8Header = FRAME_HEADER(MOTOR_NUMBER, FRAME_CODE_GET_REG);
	
	// Create the frame message
	class frame_class cGetRegFrame(ui8Header, ui8Reg, 0, NULL);
	// Get the frame 
	uint8_t *pui8Buff = cGetRegFrame.get_frame();
	uint8_t ui8BuffSize = cGetRegFrame.get_frame_size();

	int8_t i8Return = spClass.port_write(pui8Buff, ui8BuffSize);
	if(i8Return < 0) {
		cout << "get_reg() failed\n";
		return -1;
	}
	// Read the data
	uint8_t ui8PayloadSize = ui8RegSize + 3;
	uint8_t* pui8ReadBuff = new uint8_t[ui8PayloadSize];
	i8Return = spClass.port_read(pui8ReadBuff, 1);
	if(i8Return < 0) {
		cout << "reg_read() failed\n";
		return -1;
	}

	if(0xF0 == pui8ReadBuff[0]) {
		// If it is acknowledged
		i8Return = spClass.port_read(&pui8ReadBuff[1], ui8PayloadSize-1);
		if(i8Return < 0) {
			cout << "reg_read() failed\n";
			return -1;
		}	
		
		// Validate the checksum
		if(validate_checksum(&pui8ReadBuff[0], ui8PayloadSize-1, pui8ReadBuff[ui8PayloadSize-1]) < 0) {
			cout << "Invalid checksum \n";
			return -1;
		}		
		// Copy the data
		std::memcpy(i8RegVal, &pui8ReadBuff[2], ui8RegSize);
		return 0;
	}  else {
		cout << "NACK received\n";
		spClass.flush();
		return -1;
	}
	
	
	return 0;
}

int8_t  mc_interface_class::set_speed_Kp(int16_t i16KpVal) {
	return set_reg(REG_ID_SPEED_KP, sizeof(REG_TYPE_SPEED_KP), (int8_t *)&i16KpVal);
}

int8_t  mc_interface_class::set_speed_ramp(int32_t i32SpeedRampVal) {
	return set_reg(REG_ID_RAMP_FINAL_SPEED, sizeof(REG_TYPE_RAMP_FINAL_SPEED), (int8_t *)&i32SpeedRampVal);
}

int8_t  mc_interface_class::set_control_mode(int8_t i8ControlMode) {
	return set_reg(REG_ID_CONTROL_MODE, sizeof(REG_TYPE_CONTROL_MODE), (int8_t *)&i8ControlMode);
}

int8_t  mc_interface_class::set_torque_ref(int16_t i16TorqueRefVal) {
	return set_reg(REG_ID_TORQUE_REF, sizeof(REG_TYPE_TORQUE_REF), (int8_t *)&i16TorqueRefVal);
}

int8_t  mc_interface_class::set_flux_ref(int16_t i16FluxRefVal) {
	return set_reg(REG_ID_FLUX_REF, sizeof(REG_TYPE_FLUX_REF), (int8_t *)&i16FluxRefVal);
}

int16_t mc_interface_class::get_speed_Kp(void) {
	int16_t i16KpVal;
	if(get_reg(REG_ID_SPEED_KP, sizeof(REG_TYPE_SPEED_KP), (int8_t *)&i16KpVal)<0){
		cout << "get_speed_Kp() failed " << endl;
		return 0xFFFF;
	}
	// Read back the response
	return i16KpVal;
}

uint16_t mc_interface_class::get_heat_sink_temp(void) {
	uint16_t ui16Temperature;
	if(get_reg(REG_ID_HEATS_TEMP, sizeof(REG_TYPE_HEATS_TEMP), (int8_t *)&ui16Temperature)<0){
		cout << "get_heat_sink_temp() failed " << endl;
		return 0xFFFF;
	}
	// Read back the response
	return ui16Temperature;
}

uint16_t mc_interface_class::get_bus_voltage(void) {
	uint16_t ui16BusVoltage;
	if(get_reg(REG_ID_BUS_VOLTAGE, sizeof(REG_TYPE_BUS_VOLTAGE), (int8_t *)&ui16BusVoltage)<0){
		cout << "get_heat_sink_temp() failed " << endl;
		return 0xFFFF;
	}
	// Read back the response
	return ui16BusVoltage;
}

int8_t  mc_interface_class::start_stop_motor(void) {
	return execute_command(MOTOR_CMD_START_STOP);
}

int8_t  mc_interface_class::encoder_align(void) {
	return execute_command(MOTOR_CMD_ENCODER_ALIGN);
}

int8_t  mc_interface_class::fault_ack(void) {
	return execute_command(MOTOR_CMD_FAULT_ACK);
}

int8_t  mc_interface_class::start_motor(void) {
	return execute_command(MOTOR_CMD_START_MOTOR);
}

int8_t  mc_interface_class::stop_motor(void) {
	return execute_command(MOTOR_CMD_STOP_MOTOR);
}

int8_t  mc_interface_class::stop_ramp(void) {
	return execute_command(MOTOR_CMD_STOP_RAMP);
}


uint8_t  mc_interface_class::get_firmware_version(uint8_t *pui8Buffer, uint8_t ui8BuffSize) {
	// Create header
	//uint8_t ui8Header = FRAME_HEADER(MOTOR_NUMBER, FRAME_CODE_GET_FW_VERSION);
	// Compute the checksum
	//uint8_t ui8CheckSum = compute_checksum();
	return 0;
	
}