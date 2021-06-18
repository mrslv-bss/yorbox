#include <Servo.h>

Servo rservo;
Servo lservo;

#define rotationservo 110 // Degree of rotation for both servos. (up - 0, down - 1)

// Left servo - highservo
int anglehigh;  // Degree of rotation of high servo
long servotimerhigh; // High servo timer
// Right servo - lowservo
int anglelow; // Degree of rotation of low servo
long servotimerlow; // Low servo timer

void setup() {
  rservo.attach(3); //  Right servo
  lservo.attach(10); // Left servo

/*
  move_lowservo(false);  // Down right
  move_lowservo(true);  // Up right
  move_lowservo(false);  // Down right

  move_highservo(true);  // Down left
  move_highservo(false);  // Up left
  move_highservo(true);  // Down left
*/
}

void loop() {
}



//////////////////////////////////////////////////
// Right servo
//////////////////////////////////////////////////
void move_highservo(int mode) {
  if (mode == true) {
    while (anglehigh != 0)  { 
      if (millis() - servotimerhigh >= 15) {   // 15ms - delay servo smooth. The more - the smoother the movement.
        servotimerhigh = millis();
        anglehigh--;
        rservo.write(anglehigh);
      }
    }
    anglehigh = 0;
  }
  else if (mode == false) {
    while (anglehigh != rotationservo)  {     // Dont touch. Angle of rotation for high servo
      if (millis() - servotimerhigh >= 15) {   // 15ms - delay servo smooth. The more - the smoother the movement.
        servotimerhigh = millis();
        anglehigh++;
        rservo.write(anglehigh);
      }
    }
    anglehigh = rotationservo;  // Dont touch. Angle of rotation for high servo
  }
}

//////////////////////////////////////////////////
// Left servo
//////////////////////////////////////////////////
void move_lowservo(int mode)  {  
  if (mode == true) {
    while (anglelow != 0)  { 
      if (millis() - servotimerlow >= 15) {   // 15ms - delay servo smooth. The more - the smoother the movement.
        servotimerlow = millis();
        anglelow--;
        lservo.write(anglelow);
      }
    }
    anglelow = 0;
  }
  else if (mode == false) {
    while (anglelow != rotationservo)  { // Calibration necessary degree of rotation
      if (millis() - servotimerlow >= 15) {   // 15ms - delay servo smooth. The more - the smoother the movement.
        servotimerlow = millis();
        anglelow++;
        lservo.write(anglelow);
      }
    }
    anglelow = rotationservo; 
  }
}
