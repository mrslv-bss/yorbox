#include <Servo.h> // Используем библиотеку для работы с сервоприводом
#define doorangle 90 // Макс.угл поворота для центрального окна.

Servo servodoor; // Подключаем центрального окно.

int doorpos;  // Текущий угл поворота центрального окна.
long servotimerdoor; // Таймер для открытия центрального окна.


void setup() 
{ 
  servodoor.attach(8); //  Центральное окно

// * Как использовать
//  move_doorservo(false);  // Подняли
//
//  move_doorservo(true);  // Опустили
// * Как использовать
} 

void loop() {}

//////////////////////////////////////////////////
// Центральное окно
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
//////////////////////////////////////////////////
