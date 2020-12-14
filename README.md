<h1> ST Motor Control Interface Application </h1>

STM32 provides off-the-shelf motor control libraries for BLDC and PMSM motors. The motor control libraries can be generated using Motor Control Software Development Kit (MCSDK). For on-board control, motor control interface is available. To control the motor using an external microprocessor or host PC, there exists a frame communication protocol.

In frame communication protocol, STM32 or Nucleo-board acts as a slave and an external host PC acts as master. The frame communication protocol is independent of the underlying hardware. Generally, serial port over USB is used as a base hardware interface for the frame communication protocol. 

In this code, a frame communication protocol is implemented on host PC side that acts as a master. The underlying operating system is assumed to linux. The goal is to control the Nucelo boards over Rasp Pi.

A reference master control code based on the frame communication protocol is implemented in C++ and Python programming languages. Developers can either use C++ environment or Python environment.

---------------------
To compile C++ code
cd source_dir
cd Code/C++
g++ -o test_motor_interface.o *.cpp *.h

---------------------
To execute C++ code
sudo ./test_motor_interface.o


-----------------------------------
Python environment does not need any compilation. But it requires PySerial interface. Install it before the first use.

-----------------------------------

