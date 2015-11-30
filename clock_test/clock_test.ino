/* Arduino Implementation of a Clock
 * testing some stuff for a nixie clock I want to build
 * 
 * implementing a scheme that allows setting the clock with two
 * buttons, one to increment hours and one to increment minutes.
 * 
 * Chris Green - 11/30/2015
 */

#include <nixie10.h>

//global variables
int seconds = 0; //seconds value that MUST be reset back to zero every 60sec
int minutes[2] = {0,0}; //int array for minutes: tens, ones
int hours[2] = {1, 2}; //int array for hours: tens, ones

nixie10 one_min, ten_min, one_hour, ten_hour;

void setup() {
 int mones_pins[4]={2, 3, 4, 5};
 int mtens_pins[4]={6, 7, 8, 9};
 int hones_pins[4]={10, 11, 12, 13};
 int htens_pins[4]={14, 15, 16, 17};
 one_min.initialize_sn74141(mones_pins, 9);
 ten_min.initialize_sn74141(mtens_pins, 9);
 one_hour.initialize_sn74141(hones_pins, 9);
 ten_hour.initialize_sn74141(htens_pins, 9);
}

void loop() {
  if(seconds == 60){
    seconds = 0;
    if(minutes[1] == 10){
      minutes[1]=0;
      minutes[2]++;
      if(minutes[2] == 6){
       minutes[2]=0;
       hours[1]++;
       if(hours[1]==10){
        hours[1]==0;
        hours[2]++;
        if(hours[2]==3){
          hours[2]==0;
        }
       }
      }
    }
  }
  //clock update code
  one_min.set_sn74141(minutes[1]);
  ten_min.set_sn74141(minutes[2]);
  one_hour.set_sn74141(hours[1]);
  ten_hour.set_sn74141(hours[2]);
  seconds++;
  delay(1000);
}
