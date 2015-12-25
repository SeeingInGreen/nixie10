/* Arduino-driven nixie clock
 * 
 * Clock has two buttons, to set hours and minutes respectively.
 * A DS1307 real-time clock is also included in this implementation, allowing for the clock to keep more accurate time and retain it.
 * If the RTC battery dies, the clock *should* output "0:00"
 * 
 * The clock can also output data via serial
 * 
 * Chris Green - 12/24/2015
 */


#include <nixie10.h> // my nixie driver library
#include <Wire.h> // I2C library
#include "RTClib.h" // adafruit library to drive DS1307 RTC connected via I2C

#define HOURBUTTONPIN 2
#define MINUTEBUTTONPIN 3
#define CLOCKSETDELAY 400  //I like this amount of delay for setting my clock, but change it as you like
#define OFF 11 //setting a tube to a value higher than 9 turns it off

int hour_tens, hour_ones, min_tens, min_ones, hours, minutes; //some global variables to hold current time

nixie10 one_min, ten_min, one_hour, ten_hour; //tube objects
RTC_DS1307 rtc; //real-time-clock object

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; //for serial output

void setup() {
 //initialize digital pins to power the RTC
 pinMode(4, OUTPUT);
 pinMode(5, OUTPUT);
 digitalWrite(4, HIGH); //Vcc
 digitalWrite(5, LOW); //GND

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
 
 int mones_pins[4]={22, 23, 24, 25};
 int mtens_pins[4]={28, 29, 30, 31};
 int hones_pins[4]={34, 35, 36, 37};
 int htens_pins[4]={40, 41, 42, 43};
 //initialize tubes in the OFF state to make the clock look better
 one_min.initialize_sn74141(mones_pins, OFF);
 ten_min.initialize_sn74141(mtens_pins, OFF);
 one_hour.initialize_sn74141(hones_pins, OFF);
 ten_hour.initialize_sn74141(htens_pins, OFF);
}

void loop() {
  DateTime now = rtc.now();
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

  //break up current time into individual digits and convert to 12hr format
  min_ones = minutes%10;
  min_tens = (minutes - min_ones)/10;
  hour_ones = (hours>12) ? (hours-12)%10 : hours%10; //convert to 12hr format
  hour_tens = (hours>12) ? (hours - hour_ones - 12)/10 : (hours - hour_ones)/10; //convert to 12hr format
  
  one_min.set_sn74141(min_ones);
  ten_min.set_sn74141(min_tens);
  one_hour.set_sn74141(hour_ones);
  hour_tens ? ten_hour.set_sn74141(hour_tens) : ten_hour.set_sn74141(OFF); //turn the tube off instead of displaying a zero
    
}
