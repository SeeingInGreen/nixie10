/* Arduino-free nixie clock, built with a standalone ATmega328P microcontroller and shift registers
 * 
 * Clock has two buttons, to set hours and minutes respectively.
 * A DS1307 real-time clock is also included in this implementation, allowing for the clock to keep more accurate time and retain it.
 * If the RTC battery dies, the clock *should* output "0:00"
 * 
 * The clock can also output data via serial
 * 
 * Chris Green - 1/11/2016
 */


#include <nixie10.h> // my nixie driver library
#include <Wire.h> // I2C library
#include "RTClib.h" // adafruit library to drive DS1307 RTC connected via I2C

//ATMEGA328P Pin Assignments:
#define LATCHPIN 3
#define CLOCKPIN 4
#define DATAPIN 2
#define HOURBUTTONPIN 5
#define MINUTEBUTTONPIN 6
#define LEDPIN 13

#define CLOCKSETDELAY 400  //I like this amount of delay for setting my clock, but change it as you like
#define OFF 11 //setting a tube to a value higher than 9 turns it off

int hour_tens, hour_ones, min_tens, min_ones, hours, minutes; //some global variables to hold current time

nixie10 outReg; //tube-register object
RTC_DS1307 rtc; //real-time-clock object

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; //for serial output

void setup() {
 outReg.initialize_16reg(LATCHPIN, CLOCKPIN, DATAPIN); //initialize the output register array
 
 pinMode(LEDPIN, OUTPUT);
 
 //clock safety (borrowed from Adafruit example code)
 #ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
 #endif

 Serial.begin(57600);
 if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
 //set clock if RTC is not set
 if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

void loop() {
  DateTime now = rtc.now(); //take a "snapshot" of the time
  hours = (now.hour() ? now.hour() : 12); //account for the fact that in 24hr time, this is zero for 12AM
  minutes = now.minute();  

  if(digitalRead(HOURBUTTONPIN)){
    if(hours < 23)
      hours++;
    else
      hours = 1;
    rtc.adjust(DateTime(now.year(), now.month(), now.day(), hours, minutes, now.second()));
    delay(CLOCKSETDELAY);
  }

  if(digitalRead(MINUTEBUTTONPIN)){
    if(minutes < 59)
      minutes++;
    else
      minutes = 0;
    rtc.adjust(DateTime(now.year(), now.month(), now.day(), hours, minutes, now.second()));
    delay(CLOCKSETDELAY);
  }
  int time = millis();
  if(!(time%1000)){
    digitalWrite(LEDPIN, HIGH);
    delay(200);
    digitalWrite(LEDPIN, LOW);
  }

  //break up current time into individual digits and convert to 12hr format
  min_ones = minutes%10;
  min_tens = (minutes - min_ones)/10;
  hour_ones = (hours>12) ? (hours-12)%10 : hours%10; //convert to 12hr format
  hour_tens = (hours>12) ? (hours - hour_ones - 12)/10 : (hours - hour_ones)/10; //convert to 12hr format
  
  outReg.set_16reg((hour_tens ? hour_tens : OFF), hour_ones, min_tens, min_ones); //push out the current time to the register array, turning off 10s hour tube if zero.
    
}
