#include "serial_port.hpp"
#include <iomanip>

#undef DEBUG_PRINTS
// serial port access source for MacOS is taken from the below website
// https://www.pololu.com/docs/0J73/15.5
// The reference source code is wrapped in C++ to be used with 
using namespace std;

serial_port::serial_port(){
	// Do nothing
};

serial_port::~serial_port(){
	// Do nothing
};

int8_t serial_port::port_open(const char * device, uint32_t ui32BaudRate){
	// Open the serial port device
	i16Fd = open(device, O_RDWR | O_NOCTTY);
	if(i16Fd < 0) {
		cout << "Unable to open the serial port" << device << "\n";
		return -1;
	}
 
	// Flush away any bytes previously read or written.
	int result = tcflush(i16Fd, TCIOFLUSH);
	if (result) {
		// just a warning, not a fatal error
		cout << "tcflush failed";  
	}
 
	// Get the current configuration of the serial port.
	struct termios options;
	result = tcgetattr(i16Fd, &options);
	if (result)
	{
		cout << "tcgetattr failed";
		close(i16Fd);
	return -1;
	}
 
  // Turn off any options that might interfere with our ability to send and
  // receive raw binary bytes.
  options.c_iflag &= ~(INLCR | IGNCR | ICRNL | IXON | IXOFF);
  options.c_oflag &= ~(ONLCR | OCRNL);
  options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
 
  // Set up timeouts: Calls to read() will return as soon as there is
  // at least one byte available or when 100 ms has passed.
  options.c_cc[VTIME] = 1;
  options.c_cc[VMIN] = 0;
 
  // This code only supports certain standard baud rates. Supporting
  // non-standard baud rates should be possible but takes more work.
  switch (ui32BaudRate)
  {
  case 4800:   cfsetospeed(&options, B4800);   break;
  case 9600:   cfsetospeed(&options, B9600);   break;
  case 19200:  cfsetospeed(&options, B19200);  break;
  case 38400:  cfsetospeed(&options, B38400);  break;
  case 115200: cfsetospeed(&options, B115200); break;
  default:
  	std::cerr << "warning: baud rate " << ui32BaudRate << "is not supported, using 115200.\n";
    cfsetospeed(&options, B115200);
    break;
  }
  cfsetispeed(&options, cfgetospeed(&options));
 
  result = tcsetattr(i16Fd, TCSANOW, &options);
  if (result) {
    cout << "tcsetattr failed\n";
    close(i16Fd);
    return -1;
  }
 
  return i16Fd;
};

void serial_port::port_close(void){
	// Do nothing
};

int32_t serial_port::port_read(uint8_t *ui8Buff, uint32_t ui32BuffSize){
	int32_t i32received = read(i16Fd, ui8Buff, ui32BuffSize);
	if(i32received < 0) {
		cout << "failed to read from the port\n";
		return -1;
	} else if(0 == i32received) {
		cout << "serial port read timeout\n";
	} else {
#ifdef DEBUG_PRINTS
		cout << "serial port read success " << "\n";
		for(size_t i = 0; i < ui32BuffSize; ++i) {
			printf("0x%02X \t", ui8Buff[i]);
		}
		printf("\n");		
#endif
	}

	return i32received;
};

int8_t serial_port::port_write(uint8_t *ui8Buff, uint32_t ui32BuffSize){
#ifdef DEBUG_PRINTS	
	cout << "serial_port_write " <<  "\n";
	for(size_t i = 0; i < ui32BuffSize; ++i) {
		printf("0x%02X\t", ui8Buff[i]);
	}
	printf("\n");		
#endif	
	uint32_t ui32result = write(i16Fd, ui8Buff, ui32BuffSize);
	if (ui32result != ui32BuffSize) {
		cout << "failed to write to port";
		return -1;
	}
	return 0;
};

void serial_port::flush(void){
	// Flush away any bytes previously read or written.
	int result = tcflush(i16Fd, TCIOFLUSH);
	if (result) {
		// just a warning, not a fatal error
		cout << "tcflush failed";  
	}	
}