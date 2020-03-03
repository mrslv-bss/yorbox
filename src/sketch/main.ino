/*
=== Circle Storage ===

  Code writen by Miroslav Bass
  The license is free.
  For partial / full copying of the source code, indicate the original author.
  Github - https://github.com/BassTechnologies
  Github Repository - https://github.com/BassTechnologies/CircleStorage
  Contact - bassmiroslav@gmail.com
  Website - www.bassmiroslav.pro
  Thanks for reading the annotation.
  
=== Circle Storage ===
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

int limit = 15;  // Число, после которого выбирается слот
int selectedslot; // Выбран ли слот
int time; // Таймер отсчёта. Выбор слота.
int steppermode;  // Режим шагового двигателя
int stepperstatus;  // После прокрута карусели на нужную нам позицию - активируем режим ожидания нажатия для возврата на исходную точку.
int anglehigh = leftservo;  // Не трогать. Угл поворота для верхнего сервопривода 
int anglelow = rightservo;  // Не трогать. Угл поворота для нижнего сервопривода
int doorpos;  // Текущий угл поворота центрального окна.
// Правый сервопривод - lowservo
// Левый сервопривод - highservo

unsigned long oldtimer; // Замена delay #1
unsigned long secondoldtimer; // Замена delay #2
unsigned long servotimerhigh; // Таймер для левого сервопривода
unsigned long servotimerlow; // Таймер для правого сервопривода
unsigned long servotimerdoor; // Таймер для открытия центрального окна.

volatile boolean sensflag;  // Interrupt


void setup() {
  Serial.begin(9600);
// //////////////////////////////////////////////////
  // Stepper Motor
  stepper.setRPM(12);
  stepper.setSPR(4075.7728395);
  // Stepper Motor
// //////////////////////////////////////////////////
  // SensButton
  pinMode(sensbutton, INPUT_PULLUP);
  attachInterrupt(0, wakeup, RISING);
  // SensButton
// //////////////////////////////////////////////////
  // LED Display
  lcd.init();                     
  lcd.backlight();

  // Logo
  lcd.setCursor(1, 0);
  lcd.print("Circle Storage");
  lcd.setCursor(1, 1);
  lcd.print("@bass_devware");
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
  // Logo

  // LED Display
// //////////////////////////////////////////////////
  // Servo

      rservo.attach(5); //  Правый сервопривод
      lservo.attach(6); // Левый сервопривод
      servodoor.attach(4); //  Центральное окно

      // "Калибровка" сервомашинок
      move_doorservo(false);  // Подняли центральное окно
      move_doorservo(true);  // Опустили центральное окно
      move_lowservo(false);  // Опустили правый
      move_highservo(true);  // Опустили левый

  // Servo
// //////////////////////////////////////////////////

void loop() {
  
  stepper.run(); // Необходимо для шагового двигателя.
  
  ////////////////////////////////////
  // Возврат карусели на исходную точку
  ////////////////////////////////////
  if (stepperstatus)  {   // Прокрутили до нужного слота и теперь возвращаем на место.
    if  (stepper.isDone())  {
      move_highservo(true);  // Опустили левый
      move_lowservo(false);  // Опустили правый  
    }
      if  (stepper.isDone() && digitalRead(2) == HIGH)  {
        move_doorservo(true);  // Подняли центральное окно
        move_highservo(false);  // Опустили левый
        move_lowservo(true);  // Опустили правый  
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
          attachInterrupt(0, wakeup, RISING); // Без этой строки, после перезагрузки скрипт сразу войдёт в меню выбора слота.
          Reset();  // Перезагрузка после возвращение карусели на начальную позицию. Предотвращает возможные программные зависания и легче производить отладку.
        }
    }
  ////////////////////////////////////
  // Возврат карусели на исходную точку
  ////////////////////////////////////

  ////////////////////////////////////
  // Стартовое меню отсчёта времени
  ////////////////////////////////////
  if (menu) {
    detachInterrupt(0);
      if (millis() - oldtimer >= 1000) {
        oldtimer = millis();
          if (time >= limit)  { // Лимит времени достигнут, слот выбран - исполняем.
            menu = 0;
            selecting = 0;
            move_highservo(false);  // Опустили левый
            move_lowservo(true);  // Опустили правый
            autoaccept();
          } else  { // Чтобы после исполнение условия выше, действие не пошло дальше и не натворило бед. Костыль.
              time++;
              selecting = 1;
                if  (digitalRead(2) == HIGH)  { // Сбрасываем счётчик, если есть зажатие
                  time = 0;
                  lcd.setCursor(12, 1);
                  lcd.print(" ");
                }
                if (limit >= 10)  { // Работа с числами формата X/XX.
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
  // Стартовое меню отсчёта времени
  ////////////////////////////////////

  ////////////////////////////////////
  // Меню выбора слота
  ////////////////////////////////////
  if  (digitalRead(2) == HIGH && selecting)  {  // Есть нажатие из меню с отсчётом времени завершения ожидания.
    if (millis() - secondoldtimer >= 1000) {
      secondoldtimer = millis();
        if (selectedslot >= avalibleslots)  { // Если текущий слот >= допустимого числа
          selectedslot = 1;
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
  // Меню выбора слота
  ////////////////////////////////////

  ////////////////////////////////////
  // Выводим из спячки
  ////////////////////////////////////
  if (sensflag) {
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(1, 1);
    lcd.print("Circle Storage");
    lcd.setCursor(5, 0);
    lcd.print("Hello!");
    delay(1000); // Убрать
    interrupt();
    sensflag = 0;
  }
  ////////////////////////////////////
  // Выводим из спячки
  ////////////////////////////////////
}


void interrupt()  {
  lcd.clear();
    if (limit >= 10)  { // Работа с числами формата X/XX.
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
  menu = 1; // После вывода из спячки, активируем меню с выбором слота и отсчётом.
}

void autoaccept() {
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Slot #");
  lcd.setCursor(11, 0);
  lcd.print(selectedslot);
  lcd.setCursor(3, 1);
  lcd.print("Processing");
  if (stepper.isDone()) {
    move_doorservo(false);  // Подняли центральное окно 
  }
    switch(selectedslot)  {
      case 1:
        stepperstatus = 1;
          if (stepper.isDone()) {
            stepperfunc(0, 1);  // По часовой стрелке. Текущий слот.
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
            stepperfunc(180, 1);  // По часовой стрелке. Слот через один. 
          }
      break;
      case 4:
        stepperstatus = 1;
          if (stepper.isDone()) {
            stepperfunc(270, 1);  // По часовой стрелке. Последний. 
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
  sensflag = 1; // Выводим из спячки
  time = 0; // Сбрасываем счётчик времени в меню
  Serial.println("Hello!");
} // Main interrupt

//////////////////////////////////////////////////
// Правый сервопривод
//////////////////////////////////////////////////
void move_highservo(int mode) {
  if (mode == true) {
    while (anglehigh != 0)  { 
      if (millis() - servotimerhigh >= 15) {   // 15мс - задержка движущего элемента сервопривода. Чем больше - тем плавнее движение.
        servotimerhigh = millis();
        anglehigh--;
        rservo.write(anglehigh);
      }
    }
    anglehigh = 0;
  }
  else if (mode == false) {
    while (anglehigh != leftservo)  {     // см. комментарий 6 строки
      if (millis() - servotimerhigh >= 15) {   // 15мс - задержка движущего элемента сервопривода. Чем больше - тем плавнее движение.
        servotimerhigh = millis();
        anglehigh++;
        rservo.write(anglehigh);
      }
    }
    anglehigh = leftservo;  // см. комментарий 6 строки
  }
}
//////////////////////////////////////////////////

//////////////////////////////////////////////////
// Левый сервопривод
//////////////////////////////////////////////////
void move_lowservo(int modes)  {  
  if (modes == true) {
    while (anglelow != 0)  { 
      if (millis() - servotimerlow >= 15) {   // 15мс - задержка движущего элемента сервопривода. Чем больше - тем плавнее движение.
        servotimerlow = millis();
        anglelow--;
        lservo.write(anglelow);
      }
    }
    anglelow = 0;
  }
  else if (modes == false) {
    while (anglelow != leftservo)  { // Калибруем нужный градус поворота
      if (millis() - servotimerlow >= 15) {   // 15мс - задержка движущего элемента сервопривода. Чем больше - тем плавнее движение.
        servotimerlow = millis();
        anglelow++;
        lservo.write(anglelow);
      }
    }
    anglelow = leftservo; 
  }
}
//////////////////////////////////////////////////

//////////////////////////////////////////////////
// Центральное окно
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
//////////////////////////////////////////////////
