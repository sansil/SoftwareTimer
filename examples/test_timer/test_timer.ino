#include "softwareTimer.h"

//events to fire
char ev0 = 0;
char ev2 = 0;
char ev3 = 0;

Stimer mTimer0(&ev0);
Stimer mTimer1;
Stimer mTimer2(&ev2);
Stimer mTimer3(&ev3);

void setup()
{

  Serial.begin(115200);
  Serial.print("time 0:");
  Serial.println(millis());

  //time in ms
  mTimer0.start(10);
  mTimer1.start(100);
  mTimer2.start(1000);
  mTimer3.start(10000);
}

void loop()
{

  if (ev0)
  {
    Serial.print("[TIMER 0] time end: ");
    Serial.println(millis());
    ev0 = 0;
    //mTimer0.start(1000);
  }
  if (ev2)
  {
    Serial.print("[TIMER 2] time end: ");
    Serial.println(millis());
    ev2 = 0;
    //mTimer2.start(1000);
  }
  if (ev3)
  {
    Serial.print("[TIMER 3] time end: ");
    Serial.println(millis());
    ev3 = 0;
    mTimer3.start(16000);
  }

  //own event
  if (mTimer1.timeout_flag)
  {
    Serial.print("[TIMER 1] time end: ");
    Serial.println(millis());
    mTimer1.timeout_flag = 0;
    //mTimer1.start(100);
  }

  //Serial.println(debug_i);
}
