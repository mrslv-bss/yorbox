#include <Servo.h>
#define doorangle 90

Servo servodoor; 

int doorpos;  // Current degree of rotation of central window
long servotimerdoor; // Central window opening timer


void setup() 
{ 
  servodoor.attach(8); 

// * How to use
//  move_doorservo(false);  // Up
//
//  move_doorservo(true);  // Down
} 

void loop() {}

//////////////////////////////////////////////////
// Central Window
//////////////////////////////////////////////////
void move_doorservo(int mode) {
  if (mode == true) {
    while (doorpos != 0)  { 
      if (millis() - servotimerdoor >= 20) {
        servotimerdoor = millis();
        doorpos--;
        servodoor.write(doorpos);
      }
    }
    doorpos = 0;
  }
  else if (mode == false) {
    while (doorpos != doorangle)  {
      if (millis() - servotimerdoor >= 20) {
        servotimerdoor = millis();
        doorpos++;
        servodoor.write(doorpos);
      }
    }
    doorpos = doorangle;
  }
}
