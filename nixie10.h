/* nixie10.h
* A library for driving 10-number nixie tubes with Arduino and various circuit configurations
* Chris Green - 11/29/2015
*/

#ifndef nixie10_h
#define nixie10_h

#include "Arduino.h" //needed for Arduino-specific functions

class nixie10{
public:
	//initializers for sn74141 driver configuration
	void initialize_sn74141(int pins[4], int initVal); //initializes tube with given value
	void initialize_sn74141(int pins[4]); //initializes tube to zero
	void set_sn74141(int digit); //sets the tube to a 1-digit number
private:
	void _dec_to_bcd(int decimal, char *bcd); //converts a decimal to a bcd string
	char _bcd[4]; //holds the current bcd output
	char _pins[4]; //holds the 4 Arduino digital pins to drive the tube
};

#endif
  