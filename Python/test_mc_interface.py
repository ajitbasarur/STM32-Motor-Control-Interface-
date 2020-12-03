#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Dec  3 22:15:46 2020

@author: PadminiBhat
"""
import os
from mc_interface import MCInterface

# Set the serial port settings
if os.name == 'nt':
   comPort = 'COM4';
else:
    comPort = '/dev/cu.usbmodem14203';

# Open the motor object
motorObject = MCInterface(comPort, 115200, 1);

fw_version = motorObject.read_firmware_version();
print("The firmware version is ", fw_version);

# Read the speed PID parameter values
success, reg_value, erroCode = motorObject.read_speed_Kp();
if True == success:
    print("The register value is \n", reg_value);
else:
    print("Register read failed with error code", erroCode);

# Close the motor object
del motorObject;