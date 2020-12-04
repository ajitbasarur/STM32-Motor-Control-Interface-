#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Dec  3 22:15:46 2020

@author: PadminiBhat
"""
import os
from mc_interface import MCInterface
import sys

# Set the serial port settings
if os.name == 'nt':
   comPort = 'COM4';
else:
    comPort = '/dev/cu.usbmodem14203';

# Open the motor object
motorObject = MCInterface(comPort, 115200, 1);

# fw_version = motorObject.read_firmware_version();
# print("The firmware version is ", fw_version);
# print("The size of firmware version array is ", sys.getsizeof(fw_version));


# Read the speed PID parameter values
# success, erroCode = motorObject.set_speed_Ki(51);
# if True == success:
#     print("The register write is successfull\n");
# else:
#     print("Register write failed with error code", erroCode);

# success, reg_value, erroCode = motorObject.read_speed_Ki();
# if True == success:
#     print("The register value is \n", reg_value);
# else:
#     print("Register read failed with error code", erroCode);


# Read the bus voltage
success, reg_value, erroCode = motorObject.read_bus_voltage();
if False == success:
    print("Register read failed with error code", erroCode);

success, erroCode = motorObject.set_ramp_final_speed(15431);
if False == success:
    print("Register write failed with error code", erroCode);

# Encoder Align    
motorObject.exec_cmd_fault_ack();
# motorObject.exec_cmd_start_stop();
# Close the motor object
# del motorObject;