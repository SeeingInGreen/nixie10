/* nixie10.cpp
* A library for driving 10-number nixie tubes with Arduino and various circuit configurations
* Chris Green - 11/29/2015
*/

#include <nixie10.h>

void nixie10::initialize_sn74141(int pins[4], int initVal){
	_pins[0] = pins[0];
	_pins[1] = pins[1];
	_pins[2] = pins[2];
	_pins[3] = pins[3];
	for(int i = 0; i < 4; i++) pinMode(_pins[i], OUTPUT);
	set_sn74141(initVal);
}

void nixie10::initialize_sn74141(int pins[4]){
	_pins[0] = pins[0];
	_pins[1] = pins[1];
	_pins[2] = pins[2];
	_pins[3] = pins[3];
	for(int i = 0; i < 4; i++) pinMode(_pins[i], OUTPUT);
	set_sn74141(0);
}

void nixie10::set_sn74141(int digit){
	int bcd[4];
	_dec_to_bcd(digit, bcd);
	for(int i = 0; i<4; i++) digitalWrite(_pins[i], ((bcd[i]) ? HIGH : LOW));
}

void nixie10::clear_sn74141(){
	int bcd[4] = {1, 1, 1, 1};
	for(int i = 0; i<4; i++) digitalWrite(_pins[i], ((bcd[i]) ? HIGH : LOW));
}

void nixie10::_dec_to_bcd(int decimal, int* bcd){ //converts a decimal to a bcd string
	//TODO: implement a safety feature
	switch (decimal){
		case 0 : {bcd[3] = 0; bcd[2] = 0; bcd[1] = 0; bcd[0] = 0; break;}
		case 1 :	{bcd[3] = 0; bcd[2] = 0; bcd[1] = 0; bcd[0] = 1;break;}
		case 2 : {bcd[3] = 0; bcd[2] = 0; bcd[1] = 1; bcd[0] = 0;break;}
		case 3 : {bcd[3] = 0; bcd[2] = 0; bcd[1] = 1; bcd[0] = 1;break;}
		case 4 : {bcd[3] = 0; bcd[2] = 1; bcd[1] = 0; bcd[0] = 0;break;}
		case 5 : {bcd[3] = 0; bcd[2] = 1; bcd[1] = 0; bcd[0] = 1;break;}
		case 6 : {bcd[3] = 0; bcd[2] = 1; bcd[1] = 1; bcd[0] = 0;break;}
		case 7 : {bcd[3] = 0; bcd[2] = 1; bcd[1] = 1; bcd[0] = 1;break;}
		case 8 : {bcd[3] = 1; bcd[2] = 0; bcd[1] = 0; bcd[0] = 0;break;}
		case 9 : {bcd[3] = 1; bcd[2] = 0; bcd[1] = 0; bcd[0] = 1;break;}
		default: {bcd[3] = 1; bcd[2] = 1; bcd[1] = 1; bcd[0] = 1;break;}
	}
}