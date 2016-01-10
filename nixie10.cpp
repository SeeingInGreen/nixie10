/* nixie10.cpp
* A library for driving 10-number nixie tubes with Arduino and various circuit configurations
* Chris Green - 1/9/2016
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
		case 0 : {bcd[3] = 0; bcd[2] = 0; bcd[1] = 0; bcd[0] = 0;break;}
		case 1 : {bcd[3] = 0; bcd[2] = 0; bcd[1] = 0; bcd[0] = 1;break;}
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

void nixie10::initialize_16reg(int latchPin, int clockPin, int dataPin){ //these are pins connecting the 16-bit register to Arduino
	_latchPin = latchPin;
	_clockPin = clockPin;
	_dataPin = dataPin;
	pinMode(_latchPin, OUTPUT);
	pinMode(_clockPin, OUTPUT);
	pinMode(_dataPin, OUTPUT);
	set_16reg(0, 0, 0, 0); //initialize register to 0'b0000000000000000
}
void nixie10::set_16reg(int ten_hour, int one_hour, int ten_minute, int one_minute){ //set all tube values
	int ten_hour_a[4], one_hour_a[4], ten_minute_a[4], one_minute_a[4];
	//create BCD arrays to shift in
	_dec_to_bcd(ten_hour, ten_hour_a);
	_dec_to_bcd(one_hour, one_hour_a);
	_dec_to_bcd(ten_minute, ten_minute_a);
	_dec_to_bcd(one_minute, one_minute_a);
	//shift in arrays in most-to-lease significant bit order
	//TODO: find a more efficient way to do this
	for(int i = 3; i > 0; i-=1) _my_shift_out(_dataPin, _clockPin, ten_hour[i]);
	for(int j = 3; j > 0; j-=1) _my_shift_out(_dataPin, _clockPin, one_hour[j]);
	for(int k = 3; k > 0; k-=1) _my_shift_out(_dataPin, _clockPin, ten_minute[k]);
	for(int l = 3; l > 0; l-=1) _my_shift_out(_dataPin, _clockPin, one_minute[l]);
}
void nixie10::_my_shift_out(int dataPin, int clockPin, int val){  //my own implementation of Arduino's shiftOut(), 1 bit only;
	//TODO: check timing
	digitalWrite(dataPin, !!(val)); //the double negation ensures one or zero
	digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);
}