#include <CustomStepper.h>
CustomStepper stepper(8, 9, 10, 11);

int steppermode;  // Stepper mode
long steppertimer;  // Instead delay (more performance)

void setup()  {
  Serial.begin(9600);
  stepper.setRPM(12);
  stepper.setSPR(4075.7728395);
}

void loop() {
indexstepper(180, true);  // Clockwise
  if (millis() - steppertimer >= 5000) {  // Wait 5s
    steppertimer = millis();
    indexstepper(180, false);  // СounterClockwise
  }
}

void indexstepper(int tangaz, int mode)  {
  if (stepper.isDone() && steppermode == false && mode == true) {
    stepper.setDirection(CCW);
    stepper.rotateDegrees(tangaz);
    steppermode = true;
    Serial.println("По");
  }
  else if (stepper.isDone() && steppermode == true && mode == false)  {
    stepper.setDirection(CW);
    stepper.rotateDegrees(tangaz);
    steppermode = false;
    Serial.println("Против");
  }
  stepper.run();
}
