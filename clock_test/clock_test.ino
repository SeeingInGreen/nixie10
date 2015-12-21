/* Arduino Implementation of a Clock
 * testing some stuff for a nixie clock I want to build
 * 
 * implementing a scheme that allows setting the clock with two
 * buttons, one to increment hours and one to increment minutes.
 * 
 * Chris Green - 11/30/2015
 */

#include <nixie10.h>

#define HOURBUTTONPIN 2
#define MINUTEBUTTONPIN 3

//global variables - make sure these can't overflow!!!
int seconds = 0; //seconds value
int minutes[2] = {0,5}; //int array for minutes: tens, ones
int hours[2] = {1, 2}; //int array for hours: tens, ones

nixie10 one_min, ten_min, one_hour, ten_hour;
unsigned long int time1, time2; //variables to compensate for the time it takes the loop to run

void setup() {
 int mones_pins[4]={22, 23, 24, 25};
 int mtens_pins[4]={28, 29, 30, 31};
 int hones_pins[4]={34, 35, 36, 37};
 int htens_pins[4]={40, 41, 42, 43};
 one_min.initialize_sn74141(mones_pins, 9);
 ten_min.initialize_sn74141(mtens_pins, 9);
 one_hour.initialize_sn74141(hones_pins, 9);
 ten_hour.initialize_sn74141(htens_pins, 9);

 //Serial.begin(9600);
}

void loop() {
  time1 = millis(); //record pre-loop time
  if(seconds == 60){
    seconds = 0;
    minutes[1]++;
    if(minutes[1] == 10){
      minutes[1]=0;
      minutes[0]++;
      if(minutes[0] == 6){
       minutes[0]=0;
       hours[1]++;
       if(hours[0]){
         if (hours[1]==3){
           hours[1]=1;
          hours[0]=0;
         }
       }
       else{
          if (hours[1]==10){
          hours[1] = 0;
          hours[0]++;
          }
         }
        }
       }
      }
      time2 = millis(); //record post-loop time
while(digitalRead(HOURBUTTONPIN)){
       hours[1]++;
       if(hours[0]){
         if (hours[1]==3){
           hours[1]=1;
          hours[0]=0;
         }
       }
       else{
          if (hours[1]==10){
          hours[1] = 0;
          hours[0]++;
          }
         }
         delay(300);
         one_min.set_sn74141(minutes[1]);
         ten_min.set_sn74141(minutes[0]);
         one_hour.set_sn74141(hours[1]);
         hours[0] ? ten_hour.set_sn74141(hours[0]) : ten_hour.set_sn74141(11); //turn the tube off instead of displaying a zero
        }
while(digitalRead(MINUTEBUTTONPIN)){
    seconds = 0;
    minutes[1]++;
    if(minutes[1] == 10){
      minutes[1]=0;
      minutes[0]++;
    }
    if(minutes[0] == 6){
      minutes[0] = 0;
    }
    delay(300);
    one_min.set_sn74141(minutes[1]);
    ten_min.set_sn74141(minutes[0]);
    one_hour.set_sn74141(hours[1]);
    hours[0] ? ten_hour.set_sn74141(hours[0]) : ten_hour.set_sn74141(11); //turn the tube off instead of displaying a zero
  }
  //clock update code
  one_min.set_sn74141(minutes[1]);
  ten_min.set_sn74141(minutes[0]);
  one_hour.set_sn74141(hours[1]);
  hours[0] ? ten_hour.set_sn74141(hours[0]) : ten_hour.set_sn74141(11); //turn the tube off instead of displaying a zero
  seconds++;
  
  //Serial.println(time2-time1); //DEBUG
  delay((1000-(time2-time1)));
}
