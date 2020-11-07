<h1> ST Motor Control Interface Application </h1>

STM32 provides off-the-shelf motor control libraries for BLDC and PMSM motors. The motor control libraries can be generated using Motor Control Software Development Kit (MCSDK). For on-board control, motor control interface is available. To control the motor using an external microprocessor or host PC, there exists a frame communication protocol.

In frame communication protocol, STM32 or Nucleo-board acts as a slave and an external host PC acts as master. The frame communication protocol is independent of underlying hardware. Generally, serial port over USB is used as a base hardware interface for the frame communication protocol. 

In this code, a frame communication protocol is implemented on host PC side that acts as a master. The underlying operating system is assumed to linux. The goal is to control the Nucelo boards over Rasp Pi.