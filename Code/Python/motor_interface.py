# -*- coding: utf-8 -*-

# Import the serial port
from serial import Serial
import serial
import numpy as np
import os

#############################################################################
# Read functions
#############################################################################
def read_firmware_version():
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
    serialPort.write(packet);
    # read the binary data
    firmware_version = serialPort.read(32);
    return firmware_version

    
def read_register(reg, payload_length, reg_size):
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
    serialPort.write(packet);
    
    reg_response = serialPort.read(2+reg_size);
    
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
    
def read_control_mode():
    reg = 0x03;
    payload_length = 1;
    reg_size = 1;
    return read_register(reg, payload_length, reg_size);

def read_fault_flags():
    reg = 0x01;
    payload_length = 1;
    reg_size = 4;
    return read_register(reg, payload_length, reg_size);

def read_speed_ref():
    reg = 0x04;
    payload_length = 1;
    reg_size = 4;
    return read_register(reg, payload_length, reg_size);

def read_iq_speed_mode():
    reg = 0x41;
    payload_length = 1;
    reg_size = 2;
    return read_register(reg, payload_length, reg_size);

def read_ramp_final_speed():
    reg = 0x5B;
    payload_length = 1;
    reg_size = 4;
    return read_register(reg, payload_length, reg_size);

def read_speed_meas():
    reg = 0x1E;
    payload_length = 1;
    reg_size = 4;
    return read_register(reg, payload_length, reg_size);

def read_speed_Kp():
    reg = 0x05;
    payload_length = 1;
    reg_size = 2;
    return read_register(reg, payload_length, reg_size);

def read_speed_Ki():
    reg = 0x06;
    payload_length = 1;
    reg_size = 2;
    return read_register(reg, payload_length, reg_size);
    
def read_torque_ref():
    reg = 0x08;
    payload_length = 1;
    reg_size = 2;
    return read_register(reg, payload_length, reg_size);

def read_torque_meas():
    reg = 0x1F;
    payload_length = 1;
    reg_size = 2;
    return read_register(reg, payload_length, reg_size);

def read_torque_Kp():
    reg = 0x09;
    payload_length = 1;
    reg_size = 2;
    return read_register(reg, payload_length, reg_size);

def read_torque_Ki():
    reg = 0x0A;
    payload_length = 1;
    reg_size = 2;
    return read_register(reg, payload_length, reg_size);

def read_flux_ref():
    reg = 0x0C;
    payload_length = 1;
    reg_size = 2;
    return read_register(reg, payload_length, reg_size);

def read_flux_meas():
    reg = 0x20;
    payload_length = 1;
    reg_size = 2;
    return read_register(reg, payload_length, reg_size);

def read_flux_Kp():
    reg = 0x0D;
    payload_length = 1;
    reg_size = 2;
    return read_register(reg, payload_length, reg_size);

def read_flux_Ki():
    reg = 0x0E;
    payload_length = 1;
    reg_size = 2;
    return read_register(reg, payload_length, reg_size);

def read_bus_voltage():
    reg = 0x19;
    payload_length = 1;
    reg_size = 2;
    return read_register(reg, payload_length, reg_size);

def read_heat_sink_temperature():
    reg = 0x1A;
    payload_length = 1;
    reg_size = 2;
    return read_register(reg, payload_length, reg_size);


#############################################################################
# Write functions
#############################################################################
def write_register(reg, reg_size, reg_value):
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
    serialPort.write(packet);
    
    # For a positive acknowledge, there is no error code
    # It seems like, the MC interface sends double acknowlegment
    reg_response = serialPort.read(3);
    
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
        errorCode = reg_response[1];
        print("Error code is", errorCode);
        
    return success, errorCode;


def set_control_mode(value):
    reg = 0x03;
    reg_size = 1;
    return write_register(reg, reg_size, value);

def set_ramp_final_speed(value):
    reg = 0x5B;
    reg_size = 4;
    return write_register(reg, reg_size, value);

def set_speed_Kp(value):
    reg = 0x05;
    reg_size = 2;
    return write_register(reg, reg_size, value);

def set_speed_Ki(value):
    reg = 0x06;
    reg_size = 2;
    return write_register(reg, reg_size, value);

def set_torque_ref(value):
    reg = 0x08;
    reg_size = 2;
    return write_register(reg, reg_size, value);

def set_torque_Kp(value):
    reg = 0x09;
    reg_size = 2;
    return write_register(reg, reg_size, value);

def set_torque_Ki(value):
    reg = 0x0A;
    reg_size = 2;
    return write_register(reg, reg_size, value);

def set_flux_ref(value):
    reg = 0x0C;
    reg_size = 2;
    return write_register(reg, reg_size, value);

def set_flux_Kp(value):
    reg = 0x0D;
    reg_size = 2;
    return write_register(reg, reg_size, value);

def set_flux_Ki(value):
    reg = 0x0E;
    reg_size = 2;
    return write_register(reg, reg_size, value);


#############################################################################
# Execute commands
#############################################################################
def execute_command(command):
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
    serialPort.write(packet);
    
    # For a positive acknowledge, there is no error code
    # It seems like, the MC interface sends double acknowlegment
    reg_response = serialPort.read(2);
    
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
        errorCode = reg_response[1];
        print("Error code is", errorCode);
        
    return success, errorCode;

def exec_cmd_start_motor():
    return execute_command(0x01);

def exec_cmd_stop_motor():
    return execute_command(0x02);

def exec_cmd_stop_ramp():
    return execute_command(0x03);

def exec_cmd_start_stop():
    return execute_command(0x06);

def exec_cmd_fault_ack():
    return execute_command(0x07);

def exec_cmd_enc_align():
    return execute_command(0x08);

def exec_cmd_iqdref_clear():
    return execute_command(0x09);

#############################################################################
# Set RAMP
#############################################################################
def set_ramp(rampValue, rampDuration):
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
    serialPort.write(packet);
    
    print(packet)
    # For a positive acknowledge, there is no error code
    # It seems like, the MC interface sends double acknowlegment
    reg_response = serialPort.read(2);
    
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
        errorCode = reg_response[1];
        print("Error code is", errorCode);
        
    return success, errorCode;


# Set the serial port settings
if os.name == 'nt':
   comPort = 'COM4';
else:
    comPort = '/dev/cu.usbmodem14203';
    
serialPort = Serial (
               port = comPort,
               baudrate = 115200,
               parity = serial.PARITY_NONE,
               stopbits = serial.STOPBITS_ONE,
               bytesize = serial.EIGHTBITS,
               timeout = 1
           )

# Pint the name of serial port
print(serialPort.name)

# Checking if the serial port is open
if True == serialPort.isOpen():
    print("Serial port is successfully opened\n")
else:
    print("Serial port is not yet open\n");
    exit();
    

# Read the firmware version
#mc_version = read_firmware_version();
#print("The firmware version is ", mc_version);



# #success, reg_value, erroCode = read_heat_sink_temperature();
# success, erroCode = set_flux_Ki(0xFFFF);
# if True == success:
#     print("The register write is successfull\n");
# else:
#     print("Register write failed with error code", erroCode);
    
# x = serialPort.inWaiting();
# if x > 0:
#     junk_data = serialPort.read(x);
    
# success, reg_value, erroCode = read_flux_Ki();
# if True == success:
#     print("The register value is \n", reg_value);
# else:
#     print("Register read failed with error code", erroCode);
    
x = serialPort.inWaiting();
if x > 0:
    junk_data = serialPort.read(x);
    print(x)
    
# # Start the motor
# success, erroCode = exec_cmd_enc_align();    
# if True == success:
#     print("The command execution is successfull\n");
# else:
#     print("The command execution failed with an error code", erroCode);
    
set_ramp(1000, 60);
    
# Close the serial port
serialPort.close();
