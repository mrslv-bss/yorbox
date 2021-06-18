/*

  === CREDITS ===

  Miroslav Bass
  bassmiroslav@gmail.com
  Github Repository - https://github.com/mrslv-bss/yorbox
  
  === CREDITS ===

  Last stable version: [1.0] 3.5.2021
    ~ Release
  Current version: [1.1]
    ~ [1.1.1] Dev comment's localization

*/

#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <CustomStepper.h>

// Все необходимые "дополнительные" к стандартным библиотеки см. в репозитории проекта.
#define sensbutton 2 // Сенсорная кнопка
#define avalibleslots 4 // Количество слотов:
/*
Поддерживает пока только 4. Не больше/меньше. 
*/
#define Reset() asm("JMP 0")  // Программная перезагрузка
#define doorangle 90 // Макс.угл поворота для центрального окна.
#define leftservo 100 // Угл поворота для левого сервопривода + максимальный градус поворота. см.далее
/*  
По хорошему нужно инвертировать на программном уровне, но мне лень. 
Просто для опускания используем 1 (а не 0) и 0 для поднятия. И устанавливаем здесь макс.градус.
*/
#define rightservo 0  // Угл поворота для правого сервопривода 
/*
Как устанавливать нужный градус:
В лежачем положении, если движущий элемент сервопривода ближе к правому краю - значит всё нормально
т.е. инвертировать движение не нужно. Оставляем 0. 
Если смотрит в левую сторону - инвертируем работу, а как я описал в предыдущем комментарии:
Опускание это 1
Поднятие это 0
Градус ставим 110.
*/

CustomStepper stepper(8, 9, 10, 11);  // Подключаем шаговый двигатель
LiquidCrystal_I2C lcd(0x27,16,2);  // Устанавливаем дисплей

Servo rservo; // Правый сервопривод
Servo lservo; // Левый сервопривод
Servo servodoor; // Подключаем центрального окно.

boolean menu; // Находимся ли мы в меню выбора
boolean selecting; // Выключаем возможность выбора слота, после того как мы выбрали уже этот слот.

int limit = 7;  // Таймер отсчёта для выбора слота
int selectedslot; // Выбран ли слот
int time; // Таймер отсчёта. Выбор слота.
int steppermode;  // Режим шагового двигателя
int stepperstatus;  // После прокрута карусели на нужную нам позицию - активируем режим ожидания нажатия для возврата на исходную точку.
int anglehigh = leftservo;  // Dont touch. Angle of rotation for high servo 
int anglelow = rightservo;  // Dont touch. Angle of rotation for low servo 
int doorpos;  // Current angle of rotation central window.
// Right servo - lowservo
// Left servo - highservo

unsigned long oldtimer; // Замена delay #1
unsigned long secondoldtimer; // Замена delay #2
unsigned long servotimerhigh; // Таймер для левого сервопривода
unsigned long servotimerlow; // Таймер для правого сервопривода
unsigned long servotimerdoor; // Таймер для открытия центрального окна.

volatile boolean sensflag;  // Interrupt


void setup() {
  Serial.begin(9600);

  // Stepper Motor
  stepper.setRPM(12);
  stepper.setSPR(4075.7728395);

  // SensButton
  pinMode(sensbutton, INPUT_PULLUP);
  attachInterrupt(0, wakeup, RISING);

  // LED Display
  lcd.init();                     
  lcd.backlight();

  // Logo
  lcd.setCursor(1, 0);
  lcd.print("Conveyor Box");
  lcd.setCursor(1, 1);
  lcd.print("@mrslv-bss");
  delay(500);
  lcd.setCursor(0, 0);
  lcd.print("_");
  delay(500);
  lcd.setCursor(7, 0);
  lcd.print("\\");
  delay(500);
  lcd.setCursor(15, 0);
  lcd.print("_");
  delay(500);
  lcd.setCursor(7, 0);
  lcd.print(" ");
  lcd.setCursor(15, 0);
  lcd.print(" ");
  lcd.setCursor(0, 0);
  lcd.print(" ");
  delay(500);
  lcd.clear();
  delay(500);
  lcd.noBacklight();

  // LED Display
// //////////////////////////////////////////////////
  // Servo

      rservo.attach(5); //  Right servo
      lservo.attach(6); // Left servo
      servodoor.attach(4); //  Central window

      // Servos calibration
      move_doorservo(false);  // Up central windoow
      move_doorservo(true);  // Down central window
      move_lowservo(false);  // Down right
      move_highservo(true);  // Down left

  // Servo
// //////////////////////////////////////////////////
}

void loop() {
  
  stepper.run(); // Necessary for stepper
  
  ////////////////////////////////////
  // Return carusel to initial position
  ////////////////////////////////////
  if (stepperstatus)  {   // Scrolled to necessary slot and now get back.
    lcd.noBacklight();
    if  (stepper.isDone())  {
      move_highservo(true);  // Down left
      move_lowservo(false);  // Down right  
    }
      if  (stepper.isDone() && digitalRead(2) == HIGH)  {
        move_doorservo(true);  // Up central window
        move_highservo(false);  // Down Left
        move_lowservo(true);  // Down right  
        switch(selectedslot)  {
          case 1:
            stepperfunc(0, 0);
          break;
          case 2:
            stepperfunc(90, 0);
          break;
          case 3:
            stepperfunc(180, 0);
          break;
          case 4:
            stepperfunc(270, 0);
          break;
        }
        stepperstatus = 2;
      }
        if (stepper.isDone() && stepperstatus == 2) {
          attachInterrupt(0, wakeup, RISING); // Without this line, after rebooting, the script will immediately enter the slot selection menu.
          Reset();  // Reload after return carusel on initial position. Prevents possible software freezes and is easier to debug.
        }
    }

  ////////////////////////////////////
  // Start menu counter
  ////////////////////////////////////
  if (menu) {
    detachInterrupt(0);
      if (millis() - oldtimer >= 1000) {
        oldtimer = millis();
          if (time >= limit)  { // Time limit reached, slot choosed - execute.
            lcd.noBacklight();
            menu = false;
            selecting = false;
            move_highservo(false);  // Down left
            move_lowservo(true);  // Down right
            autoaccept();
          } else  { // 
              time++;
              selecting = true;
                if  (digitalRead(2) == HIGH)  { // Reset time counter, if button press catched
                  time = 0;
                  lcd.setCursor(12, 1);
                  lcd.print(" ");
                }
                if (limit >= 10)  { // Work with numbers format X/XX.
                  lcd.setCursor(11, 1);
                } else  {
                    lcd.setCursor(12, 1);
                }
            lcd.print(time);
            lcd.setCursor(13, 1);
            lcd.print("/");
            lcd.setCursor(14, 1);
            lcd.print(limit);
          }
      }
  }

  ////////////////////////////////////
  // Select slot menu
  ////////////////////////////////////
  if  (digitalRead(2) == HIGH && selecting)  {  // Press on menu with time counter.
    if (millis() - secondoldtimer >= 1000) {
      secondoldtimer = millis();
        if (selectedslot >= avalibleslots)  { // If current slot >= allowable number
          selectedslot = 1; // Lower choosed slot on 1, as see comment above.
        } else  {
            selectedslot++;
          }
    lcd.setCursor(0, 0);
    lcd.print("Selected:");
    lcd.setCursor(10, 0);
    lcd.print(selectedslot);
    lcd.setCursor(12, 0);
    lcd.print("slot");
    }
  }

  ////////////////////////////////////
  // Get out of sleep
  ////////////////////////////////////
  if (sensflag) {
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(1, 1);
    lcd.print("Circle Storage");
    lcd.setCursor(5, 0);
    lcd.print("Hello!");
    delay(1000); // 50/50. You can remove it, or not. On your opinion.
    interrupt();
    sensflag = false;
  }
}


void interrupt()  {
  lcd.clear();
    if (limit >= 10)  { // Work with numbers format X/XX.
      lcd.setCursor(0, 1);
      lcd.print("AutoAccept:");
    } else  {
        lcd.setCursor(0, 1);
        lcd.print("Auto Accept:");
      }
  selectedslot++;
  lcd.setCursor(0, 0);
  lcd.print("Selected:");
  lcd.setCursor(10, 0);
  lcd.print(selectedslot);
  lcd.setCursor(12, 0);
  lcd.print("slot");
  menu = true; // After get out of sleep, activate select slot menu with counter.
}

void autoaccept() {
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Slot #");
  lcd.setCursor(11, 0);
  lcd.print(selectedslot);
  lcd.setCursor(3, 1);
  lcd.print("Processing");
  if (stepper.isDone()) {
    move_doorservo(false);  // Up central window. 
  }
    switch(selectedslot)  {
      case 1:
        stepperstatus = 1;
          if (stepper.isDone()) {
            stepperfunc(0, 1);  // Clockwise. Current slot.
          }
      break;
      case 2:
        stepperstatus = 1;
          if (stepper.isDone()) {
            stepperfunc(90, 1);
          }
      break;
      case 3:
        stepperstatus = 1;
          if (stepper.isDone()) {
            stepperfunc(180, 1);  // Clockwise. Slot in one. 
          }
      break;
      case 4:
        stepperstatus = 1;
          if (stepper.isDone()) {
            stepperfunc(270, 1);  // Clockwise. Last. 
          }
      break;
    }
  }

void stepperfunc(int tangaz, int wwd)  {
  if (stepper.isDone() && steppermode == false && wwd == true)
  {
    stepper.setDirection(CCW);
    stepper.rotateDegrees(tangaz);
    steppermode = true;
    Serial.println("По");
  } else if (stepper.isDone() && steppermode == true && wwd == false) {
      stepper.setDirection(CW);
      stepper.rotateDegrees(tangaz);
      steppermode = false;
      Serial.println("Против");
    }
  }

void wakeup() { // Main interrupt
  sensflag = true; // Get out of sleep
  time = 0; // Reset counter time menu
  Serial.println("Hello!");
} // Main interrupt

//////////////////////////////////////////////////
// Right Servo
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
    while (anglehigh != leftservo)  {     // Dont touch. Angle of rotation for high servo
      if (millis() - servotimerhigh >= 15) {   // 15ms - delay servo smooth. The more - the smoother the movement.
        servotimerhigh = millis();
        anglehigh++;
        rservo.write(anglehigh);
      }
    }
    anglehigh = leftservo;  // Dont touch. Angle of rotation for high servo
  }
}

//////////////////////////////////////////////////
// Left Servo
//////////////////////////////////////////////////
void move_lowservo(int modes)  {  
  if (modes == true) {
    while (anglelow != 0)  { 
      if (millis() - servotimerlow >= 15) {   // 15ms - delay servo smooth. The more - the smoother the movement.
        servotimerlow = millis();
        anglelow--;
        lservo.write(anglelow);
      }
    }
    anglelow = 0;
  }
  else if (modes == false) {
    while (anglelow != leftservo)  { // Calibration degree of rotation
      if (millis() - servotimerlow >= 15) {   // 15ms - delay servo smooth. The more - the smoother the movement.
        servotimerlow = millis();
        anglelow++;
        lservo.write(anglelow);
      }
    }
    anglelow = leftservo; 
  }
}

//////////////////////////////////////////////////
// Central Window
//////////////////////////////////////////////////
void move_doorservo(int moded) {
  if (moded == true) {
    while (doorpos != 0)  { 
      if (millis() - servotimerdoor >= 20) {
        servotimerdoor = millis();
        doorpos--;
        servodoor.write(doorpos);
      }
    }
    doorpos = 0;
  }
  else if (moded == false) {
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
