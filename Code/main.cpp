#include <cstdio>
#include <iostream>

#include "mc_interface.hpp"

using namespace std;


int main(){
	class mc_interface_class mcInterface;

	cout << "Starting the application \n";
	mcInterface.init("/dev/cu.usbmodem14203");
	cout << "Initialization done \n";
	mcInterface.set_speed_Kp(1200);

	cout << "Ending the application \n";

	return 0;
};