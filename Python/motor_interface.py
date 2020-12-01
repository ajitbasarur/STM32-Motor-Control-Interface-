# -*- coding: utf-8 -*-

# Import the serial port
from serial import Serial
import serial
import numpy as np


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

    
def read_register(reg, payload_length):
    header = 0x02;
    payload = reg;
    checksum = header + payload + payload_length;
    
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
    
    reg_response = serialPort.read(1+payload_length);
    
    return reg_response;
    
# Set the serial port settings
serialPort = Serial (
               port = 'COM4',
               baudrate = 115200,
               parity = serial.PARITY_NONE,
               stopbits = serial.STOPBITS_ONE,
               bytesize = serial.EIGHTBITS,
               timeout = 1000
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

reg = 0x05;
payload_length = 2;
reg_value = read_register(reg, payload_length);
print("The register ", reg, " value is ",reg_value);
    
# Close the serial port
serialPort.close();
