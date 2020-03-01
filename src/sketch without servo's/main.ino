#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <CustomStepper.h>
#define sensbutton 2 // Сенсорная кнопка
#define avalibleslots 4 // Количество слотов:
/*
Поддерживает пока только 4. Не больше/меньше. 
*/
#define Reset() asm("JMP 0")  // Программная перезагрузка

CustomStepper stepper(8, 9, 10, 11);  // Подключаем шаговый двигатель
LiquidCrystal_I2C lcd(0x27,16,2);  // Устанавливаем дисплей

boolean menu; // Находимся ли мы в меню выбора
boolean selecting; // Выключаем возможность выбора слота, после того как мы выбрали уже этот слот.

int limit = 15;  // Число, после которого выбирается слот
int selectedslot; // Выбран ли слот
int time; // Таймер отсчёта. Выбор слота.
int steppermode;  // Режим шагового двигателя
int stepperstatus;  // После прокрута карусели на нужную нам позицию - активируем режим ожидания нажатия для возврата на исходную точку.

unsigned long oldtimer; // Замена delay #1
unsigned long secondoldtimer; // Замена delay #2

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

  // Версия без сервоприводов

  // Servo
// //////////////////////////////////////////////////
}

void loop() {
  
  stepper.run(); // Необходимо для шагового двигателя.
  
  ////////////////////////////////////
  // Возврат карусели на исходную точку
  ////////////////////////////////////
  if (stepperstatus)  {   // Прокрутили до нужного слота и теперь возвращаем на место.
    if  (stepper.isDone() && digitalRead(2) == HIGH)  {  
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
