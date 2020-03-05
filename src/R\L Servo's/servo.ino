#include <Servo.h> // Используем библиотеку для работы с сервоприводом

Servo rservo; // Правый сервопривод
Servo lservo; // Левый сервопривод

#define rotationservo 110 // Угл поворота для обеих сервоприводов. см.далее
/*  
По хорошему нужно инвертировать на программном уровне, но мне лень. 
Просто для опускания правого используем 1 (а не 0 как для левого) и 0 для поднятия (а не 1 как для левого). 
*/

/*
Как устанавливать нужный градус.
В лежачем положении, если движущий элемент сервопривода ближе к правому краю - значит всё нормально
т.е. инвертировать движение не нужно. Оставляем 0. 
Если смотрит в левую сторону - инвертируем работу, а как я описал в предыдущем комментарии:
Опускание это 1
Поднятие это 0
Градус ставим 110.
*/

// Левый сервопривод - highservo
int anglehigh;  // Не трогать. Угл поворота для верхнего сервопривода 
long servotimerhigh; // Таймер для верхнего сервопривода
// Правый сервопривод - lowservo
int anglelow; // Угл поворота для нижнего сервопривода
long servotimerlow; // Таймер для нижнего сервопривода

void setup() {
  rservo.attach(3); //  Правый сервопривод
  lservo.attach(10); // Левый сервопривод

/*\
 * Использование
move_lowservo(false);  // Опустили правый
move_lowservo(true);  // Подняли правый
move_lowservo(false);  // Опустили правый

move_highservo(true);  // Опустили левый
move_highservo(false);  // Подняли левый
move_highservo(true);  // Опустили  левый
*/
}

void loop(){
  //
}



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
    while (anglehigh != rotationservo)  {     // см. комментарий 6 строки
      if (millis() - servotimerhigh >= 15) {   // 15мс - задержка движущего элемента сервопривода. Чем больше - тем плавнее движение.
        servotimerhigh = millis();
        anglehigh++;
        rservo.write(anglehigh);
      }
    }
    anglehigh = rotationservo;  // см. комментарий 6 строки
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
    while (anglelow != rotationservo)  { // Калибруем нужный градус поворота
      if (millis() - servotimerlow >= 15) {   // 15мс - задержка движущего элемента сервопривода. Чем больше - тем плавнее движение.
        servotimerlow = millis();
        anglelow++;
        lservo.write(anglelow);
      }
    }
    anglelow = rotationservo; 
  }
}
//////////////////////////////////////////////////
