/* Arduino Implementation of a Clock
 * testing some stuff for a nixie clock I want to build
 * 
 * implementing a scheme that allows setting the clock with two
 * buttons, one to increment hours and one to increment minutes.
 * 
 * Chris Green - 11/30/2015
 */

//global variables
int seconds = 0; //seconds value that MUST be reset back to zero every 60sec
int minutes[2] = {0,0}; //int array for minutes: tens, ones
int hours[2] = {1, 2}; //int array for hours: tens, ones

void setup() {

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
  //clock update code here
  seconds++;
  delay(1000);
}
