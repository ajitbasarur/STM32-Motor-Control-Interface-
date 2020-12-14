#ifndef __SERIAL_PORT_HPP__
#define __SERIAL_PORT_HPP__

#include <iostream>
#include <cstdio>
#include <cstdint>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

class serial_port {
public:
	// Constructor
	serial_port();
	// Destructor
	~serial_port();
	int8_t port_open(const char * device, uint32_t ui32BaudRate);
	void port_close(void);
	int32_t port_read(uint8_t *ui8Buff, uint32_t ui32BuffSize);
	int8_t port_write(uint8_t *ui8Buff, uint32_t ui32BuffSize);
	void flush(void);

private:
	int16_t i16Fd;

	
};

#endif //__SERIAL_PORT_HPP__