#include <CustomStepper.h>  // Используем библиотеку
CustomStepper stepper(8, 9, 10, 11);  // Подключаем

int steppermode;  // Режим шагового двигателя
long steppertimer;  // Замена delay

void setup()
{
  Serial.begin(9600);
  stepper.setRPM(12);
  stepper.setSPR(4075.7728395);
}

void loop()
{

first(180, true);  // Идём по часовой
if (millis() - steppertimer >= 5000) {  // Ждём 5 секунд
  steppertimer = millis();
  first(180, false);  // Идём против часовой
}

}

void first(int tangaz, int wwd)  {
  if (stepper.isDone() && steppermode == false && wwd == true)
  {
    stepper.setDirection(CCW);
    stepper.rotateDegrees(tangaz);
    steppermode = true;
    Serial.println("По");
  }
  else if (stepper.isDone() && steppermode == true && wwd == false)
  {
    stepper.setDirection(CW);
    stepper.rotateDegrees(tangaz);
    steppermode = false;
    Serial.println("Против");
  }
  stepper.run();
}
