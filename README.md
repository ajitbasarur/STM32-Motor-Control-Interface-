# ST Motor Control Interface Application

<p> STM32 provides off-the-shelf motor control libraries for BLDC and PMSM motors. The motor control libraries can be generated using Motor Control Software Development Kit (MCSDK). For on-board control, motor control interface is available. To control the motor using an external microprocessor or host PC, there exists a frame communication protocol. </p>

<p> In frame communication protocol, STM32 or Nucleo-board acts as a slave and an external host PC acts as master. The frame communication protocol is independent of the underlying hardware. Generally, serial port over USB is used as a base hardware interface for the frame communication protocol. </p>

<p> In this code, a frame communication protocol is implemented on host PC side that acts as a master. The underlying operating system is assumed to linux. The goal is to control the Nucelo boards over Rasp Pi. </p>

<p> A reference master control code based on the frame communication protocol is implemented in C++ and Python programming languages. Developers can either use C++ environment or Python environment. </p>

---------------------
## To compile and execute C++ code: <br>
<ol>
<li>cd source_dir</li>
<li>cd Code/C++</li>
<li>g++ -o test_motor_interface.o *.cpp *.h</li>
</ol> 
<br>

To execute C++ code <br>
sudo ./test_motor_interface.o help <br>

-----------------------------------
## To test in Python environment <br>
Python environment does not need any compilation. But it requires PySerial interface. Install it before the first use. <br>

-----------------------------------
## Testing environment <br>
1. [Nucelo-F746ZG](https://www.st.com/en/evaluation-tools/nucleo-f746zg.html) + [IHM08M1](https://www.st.com/en/ecosystems/x-nucleo-ihm08m1.html) motor controller board is used.
2. A standard BLDC motor with an external magnetic resolver. 
3. [Motor Control Workbech 5.4.4.](https://www.st.com/en/embedded-software/x-cube-mcsdk.html) is used for motor and board configuration.
4. Ubuntu 18.04 LTS is used as a host PC.
5. Nucelo-F746ZG is connected to the host PC using USB port.


