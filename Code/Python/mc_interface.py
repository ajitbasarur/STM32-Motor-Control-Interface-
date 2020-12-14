#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Dec  3 20:41:49 2020

@author: Ajit Basarur
"""
from serial import Serial
import serial
import math

class MCInterface:
    
    def __init__(self, comPort, baudrate, timeout):
        self.port = comPort;
        self.baudrate = baudrate;
        self.parity = serial.PARITY_NONE;
        self.stopbits = serial.STOPBITS_ONE;
        self.bytesize = serial.EIGHTBITS;
        if -1 == timeout:
            self.timeout = None
        else:
            self.timeout = timeout;
      
        self.serialPort = Serial (
                 port = self.port,
                 baudrate = self.baudrate,
                 parity = self.parity,
                 stopbits = self.stopbits,
                 bytesize = self.bytesize,
                 timeout = self.timeout
             )
      
          # Checking if the serial port is open
        if True == self.serialPort.isOpen():
            print("Serial port is successfully opened\n")
        else:
            print("Serial port is not yet open\n");
        
        def __del__(self):
            # body of destructor
            # Close the serial port
            self.serialPort.close();
            print("Serial port is successfully closed\n");
            
    def read_junk_data(self):
        x = self.serialPort.inWaiting();
        if x > 0:
            junk_data = self.serialPort.read(x);
            print("Junk data", junk_data)
        
    ###########################################################################
    # Read Firmware
    ###########################################################################
    def read_firmware_version(self):
        
        self.read_junk_data();
        header = 0x06;
        payload_length = 0;
        checksum = header + payload_length;
        
        packet = bytearray();
        # Append the header
        packet.append(header);
        # Append the length
        packet.append(payload_length);
        # Append the checksum
        packet.append(checksum);
        self.serialPort.write(packet);
        # read the binary data
        firmware_version = self.serialPort.read(35);
        
        return firmware_version       

    ###########################################################################
    # Read register
    ###########################################################################
    def read_register(self, reg, payload_length, reg_size):
        self.read_junk_data();
        
        header = 0x02;
        payload = reg;
        checksum = header + payload + payload_length;
        errorCode = 0;
        
        packet = bytearray();
        # Append the header
        packet.append(header);
        # Append the length
        packet.append(payload_length);
        # Append the payload
        packet.append(reg);
        # Append the checksum
        packet.append(checksum);
        self.serialPort.write(packet);
        
        reg_response = self.serialPort.read(3+reg_size);
        
        print(reg_response);
        ack = reg_response[0];
        success = False;
        if 0xF0  == ack:
            success = True;
            reg_value = 0;
            for i in range(reg_size):
                # Left shift each byte by 8-bits
                reg_value = reg_value | (reg_response[i+2] & 0xFF) << (8*i);
        else:
            reg_value = 0xFF;
            errorCode = reg_response[1];
            print("Register response", reg_response);
            print("Error code is", errorCode);
            
        return success, reg_value, errorCode;
    
    ###########################################################################
    # Write register
    ###########################################################################
    def write_register(self, reg, reg_size, reg_value):
        self.read_junk_data();
        header = 0x01;
        payload = reg;
        # Payload also contains the register to be written. Hence, it is +1
        payload_length = 1 + reg_size;
        Accumulator = header + payload + payload_length;
        
        # Add register value to the checksum
        for i in range(0, reg_size):
            Accumulator = Accumulator + ((reg_value >> (8*i)) & 0xFF);
        
        checksum = ((Accumulator & 0xFF) + ((Accumulator >> 8) & 0xFF)) & 0xFF;    
        print("Checksum value is ", checksum)
        errorCode = 0;
        
        packet = bytearray();
        # Append the header
        packet.append(header);
        # Append the length
        packet.append(payload_length);
        # Append the payload
        packet.append(reg);
        for i in range(0, reg_size):
            packet.append((reg_value>> (8*i)) & 0xFF);
        # Append the checksum
        packet.append(checksum);
        self.serialPort.write(packet);
        
        # For a positive acknowledge, there is no error code
        # It seems like, the MC interface sends double acknowlegment
        reg_response = self.serialPort.read(3);
        
        success = False;
        print(reg_response);
        ack = reg_response[0];
        success = False;
        if 0xF0  == ack:
            success = True;
            print("Register write is successfull\n")
        else:
            # For a negative acknowledge, there is an associated errorcode
            # Read the error code
            #reg_response = serialPort.read(1);
            errorCode = reg_response[2];
            print("Error code is", errorCode);
            # Because of error, the response contains one more message byte
            # This byte corresponds to CRC
            reg_response = self.serialPort.read(1);
            
        return success, errorCode;
    
    ###########################################################################
    # Execute commands
    ###########################################################################
    def execute_command(self, command):
        self.read_junk_data();
        header = 0x03;
        payload = command;
        payload_length = 1;
        Accumulator = header + payload + payload_length;
        
        checksum = ((Accumulator & 0xFF) + ((Accumulator >> 8) & 0xFF)) & 0xFF;    
        print("Checksum value is ", checksum)
        errorCode = 0;
        
        packet = bytearray();
        # Append the header
        packet.append(header);
        # Append the length
        packet.append(payload_length);
        # Append the payload
        for i in range(0, payload_length):
            packet.append((payload >> (8*i)) & 0xFF);
        # Append the checksum
        packet.append(checksum);
        self.serialPort.write(packet);
        
        # For a positive acknowledge, there is no error code
        # It seems like, the MC interface sends double acknowlegment
        reg_response = self.serialPort.read(3);
        
        success = False;
        print(reg_response);
        ack = reg_response[0];
        success = False;
        if 0xF0  == ack:
            success = True;
            print("Command execution is successfull\n")
        else:
            # For a negative acknowledge, there is an associated errorcode
            # Read the error code
            #reg_response = serialPort.read(1);
            errorCode = reg_response[2];
            print("Error code is", errorCode);
            # Because of error, the response contains one more message byte
            # This byte corresponds to CRC
            reg_response = self.serialPort.read(1);
            
        return success, errorCode;    
    
    ###########################################################################
    # Set RAMP
    ###########################################################################
    def set_ramp(self,rampValue, rampDuration):
        self.read_junk_data();
        header = 0x07;
        payload = bytearray();
        m = rampValue.to_bytes(4, byteorder='big', signed=False);
        n = rampDuration.to_bytes(2, byteorder='big', signed=False);
        payload = m+n
        print(payload.hex())
        payload_length = 6;
        Accumulator = header + payload_length + sum(payload);
            
        # Add register value to the checksum
        #checksum = ((Accumulator & 0xFF) + ((Accumulator >> 8) & 0xFF)) & 0xFF;
        checksum = sum(Accumulator.to_bytes(2, 'big'))    
        print("Checksum value is ", checksum)
        errorCode = 0;
    
        packet = bytearray();
        # Append the header
        packet.append(header);
        # Append the length
        packet.append(payload_length);
        # Append the payload
        packet = packet + payload;
        # Append the checksum
        packet.append(checksum);
        self.serialPort.write(packet);
        
        print(packet)
        # For a positive acknowledge, there is no error code
        # It seems like, the MC interface sends double acknowlegment
        reg_response = self.serialPort.read(2);
        
        success = False;
        print(reg_response);
        ack = reg_response[0];
        success = False;
        if 0xF0  == ack:
            success = True;
            print("Ramp Setup is successfull\n")
        else:
            # For a negative acknowledge, there is an associated errorcode
            # Read the error code
            #reg_response = serialPort.read(1);
            errorCode = reg_response[2];
            print("Error code is", errorCode);
            # Because of error, the response contains one more message byte
            # This byte corresponds to CRC
            reg_response = self.serialPort.read(1);
            
        return success, errorCode;

    ###########################################################################
    # Read registers --> accessible 
    ###########################################################################
    def read_control_mode(self):
        reg = 0x03;
        payload_length = 1;
        reg_size = 1;
        return self.read_register(reg, payload_length, reg_size);
    
    def read_fault_flags(self):
        reg = 0x01;
        payload_length = 1;
        reg_size = 4;
        return self.read_register(reg, payload_length, reg_size);
    
    def read_speed_ref(self):
        reg = 0x04;
        payload_length = 1;
        reg_size = 4;
        return self.read_register(reg, payload_length, reg_size);
    
    def read_iq_speed_mode(self):
        reg = 0x41;
        payload_length = 1;
        reg_size = 2;
        return self.read_register(reg, payload_length, reg_size);
    
    def read_ramp_final_speed(self):
        reg = 0x5B;
        payload_length = 1;
        reg_size = 4;
        return self.read_register(reg, payload_length, reg_size);
    
    def read_speed_meas(self):
        reg = 0x1E;
        payload_length = 1;
        reg_size = 4;
        return self.read_register(reg, payload_length, reg_size);
    
    def read_speed_Kp(self):
        reg = 0x05;
        payload_length = 1;
        reg_size = 2;
        return self.read_register(reg, payload_length, reg_size);
    
    def read_speed_Ki(self):
        reg = 0x06;
        payload_length = 1;
        reg_size = 2;
        return self.read_register(reg, payload_length, reg_size);
        
    def read_torque_ref(self):
        reg = 0x08;
        payload_length = 1;
        reg_size = 2;
        return self.read_register(reg, payload_length, reg_size);
    
    def read_torque_meas(self):
        reg = 0x1F;
        payload_length = 1;
        reg_size = 2;
        return self.read_register(reg, payload_length, reg_size);
    
    def read_torque_Kp(self):
        reg = 0x09;
        payload_length = 1;
        reg_size = 2;
        return self.read_register(reg, payload_length, reg_size);
    
    def read_torque_Ki(self):
        reg = 0x0A;
        payload_length = 1;
        reg_size = 2;
        return self.read_register(reg, payload_length, reg_size);
    
    def read_flux_ref(self):
        reg = 0x0C;
        payload_length = 1;
        reg_size = 2;
        return self.read_register(reg, payload_length, reg_size);
    
    def read_flux_meas(self):
        reg = 0x20;
        payload_length = 1;
        reg_size = 2;
        return self.read_register(reg, payload_length, reg_size);
    
    def read_flux_Kp(self):
        reg = 0x0D;
        payload_length = 1;
        reg_size = 2;
        return self.read_register(reg, payload_length, reg_size);
    
    def read_flux_Ki(self):
        reg = 0x0E;
        payload_length = 1;
        reg_size = 2;
        return self.read_register(reg, payload_length, reg_size);
    
    def read_bus_voltage(self):
        reg = 0x19;
        payload_length = 1;
        reg_size = 2;
        return self.read_register(reg, payload_length, reg_size);
    
    def read_heat_sink_temperature(self):
        reg = 0x1A;
        payload_length = 1;
        reg_size = 2;
        return self.read_register(reg, payload_length, reg_size);

    ###########################################################################
    # Write registers --> accessible 
    ###########################################################################
    def set_control_mode(self, value):
        reg = 0x03;
        reg_size = 1;
        return self.write_register(reg, reg_size, value);
    
    def set_ramp_final_speed(self, value):
        reg = 0x5B;
        reg_size = 4;
        return self.write_register(reg, reg_size, value);
    
    def set_speed_Kp(self, value):
        reg = 0x05;
        reg_size = 2;
        return self.write_register(reg, reg_size, value);
    
    def set_speed_Ki(self, value):
        reg = 0x06;
        reg_size = 2;
        return self.write_register(reg, reg_size, value);
    
    def set_torque_ref(self, value):
        reg = 0x08;
        reg_size = 2;
        return self.write_register(reg, reg_size, value);
    
    def set_torque_Kp(self, value):
        reg = 0x09;
        reg_size = 2;
        return self.write_register(reg, reg_size, value);
    
    def set_torque_Ki(self, value):
        reg = 0x0A;
        reg_size = 2;
        return self.write_register(reg, reg_size, value);
    
    def set_flux_ref(self, value):
        reg = 0x0C;
        reg_size = 2;
        return self.write_register(reg, reg_size, value);
    
    def set_flux_Kp(self, value):
        reg = 0x0D;
        reg_size = 2;
        return self.write_register(reg, reg_size, value);
    
    def set_flux_Ki(self, value):
        reg = 0x0E;
        reg_size = 2;
        return self.write_register(reg, reg_size, value);

    ###########################################################################
    # Execute commands --> accessible 
    ###########################################################################
    def exec_cmd_start_motor(self):
        return self.execute_command(0x01);
    
    def exec_cmd_stop_motor(self):
        return self.execute_command(0x02);
    
    def exec_cmd_stop_ramp(self):
        return self.execute_command(0x03);
    
    def exec_cmd_start_stop(self):
        return self.execute_command(0x06);
    
    def exec_cmd_fault_ack(self):
        return self.execute_command(0x07);
    
    def exec_cmd_enc_align(self):
        return self.execute_command(0x08);
    
    def exec_cmd_iqdref_clear(self):
        return self.execute_command(0x09);
    
    ###########################################################################
    # Create utility functions
    ###########################################################################    
    
    def meas_current(self):
        [s, iq, e] = self.read_flux_meas();
        [s, id, e] = self.read_torque_meas();
        # Rescale current values
        iq = iq/10000;
        id = id/10000;
        i = math.sqrt(iq**2 + id**2);
        
        return i;
        
    
    