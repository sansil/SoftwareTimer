#include "softwareTimer.h"

char ev0 = 0;
char ev2 =0;
char ev3 =0;

Stimer timer0(&ev0);
Stimer mTimer1;
Stimer mTimer2(&ev2);
Stimer timer3(&ev3);

void setup() {
  
  Serial.begin(115200);
  Serial.print("tiempo 0:");
  Serial.println(millis());

  timer0.start(10);
  mTimer1.start(100);
  mTimer2.start(1000);
  timer3.start(10);
}

void loop() {

  if(ev0){
    Serial.print("[TIMER 0] tiempo end: ");
    Serial.println(millis());
    ev0=0;
    timer0.start(1000);
  }
  if(ev2){
    Serial.print("[TIMER 2] tiempo end: ");
    Serial.println(millis());
    ev2=0;
    mTimer2.start(1000);
  }
  if(ev3){
    Serial.print("[TIMER 3] tiempo end: ");
    Serial.println(millis());
    ev3=0;
    timer3.start(16000);
  }
  if(mTimer1.timeout_flag){
    Serial.print("[TIMER 1] tiempo end: ");
    Serial.println(millis());
    mTimer1.timeout_flag = 0;
    mTimer1.start(100);
  }

  //Serial.println(debug_i);
}
