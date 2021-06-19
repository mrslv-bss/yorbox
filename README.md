![Image alt](https://github.com/mrslv-bss/yorbox/raw/master/img/conveyorbox.png)
# Conveyor Box

>Conveyor Box - The items delivery system based arduino. 3D printed parts, wooden carcass and acrylic windows.

There are 5 boxes on board. The conveyor mechanism will deliver the required box to the central mechanics window for you to pick it up.

Compared to other storage methods, the Conveyor looks futuristic, expensive and technologically advanced. 😁

![Image alt](https://img.shields.io/badge/Sketch%20memory-34%25-green) ![Image alt](https://img.shields.io/github/last-commit/mrslv-bss/yorbox)

# How It Work #

Scroll the encoder from 1 to 4 slot that appears in the display.

Single click - confirm, hold click - reset the timer for choose.
If time is up, the arduino falls asleep.
** (In actual version, instead of the encoder, we use the button) **
Single press - next slot, hold pressd - reset timer for choose.
If the time is up, your choice confirmed and the algorithm starts.

The conveyor will move to the confirmed slot.

To end the session, press the encoder (button) once.

The center window will close and the conveyor will return to its initial position.

<hr>

A list of components:
1. LCD Display 2x16 (**x4 F-M adapter** (**x8** for wire extension))
2. Sensor buttons (**x3 F-M adapter** (**x6** for wire extension))
3. Servos (**x3**)
4. Stepper (**x1**)
5. Stepper driver (**x1**)
6. Arduino Nano/Uno (**x1**)

<hr>

###### Remaster Edition ######
<img src="https://raw.githubusercontent.com/mrslv-bss/yorbox/master/img/118761516_111360574025919_1834713845074961977_o.jpg" width="150%">

###### Watch a video (download) ######
[<img src="https://raw.githubusercontent.com/mrslv-bss/yorbox/master/img/preview.png" width="150%">](https://github.com/mrslv-bss/yorbox/blob/master/img/128597729_3788562921188669_8593680395783432028_n.mp4?raw=true)

# INSTALLATION // Getting Started #

If you haven't changed the sketch pins, connect pins by instructions :

Sensor button | Right servo | Left servo | Central servo | Stepper | LCD with i2c
--- | --- | --- | --- | --- | ---
D2 | D5 | D6 | D4 | 8, 9, 10, 11 | (SDA) A4, (SCL) A5

![Image alt](https://github.com/mrslv-bss/yorbox/raw/master/img/Scheme.png)

# Libraries #

Upload additional [libraries](https://github.com/mrslv-bss/yorbox/tree/master/libs/ "Libraries") from repository.

Move uploaded libraries into:

| Documents       | x64                | x32 |
| ------------- |:------------------:| :------------------: |
| C:\Users\User\Documents\Arduino\libraries     | C:\Program Files (x86)\Arduino\libraries    | C:\Program Files\Arduino\libraries |

`Additional libraries - CustomStepper & LiquidCrystal_I2C. Other libraries are standard`

# Sketch #

1. Download `src/main.ino`
2. Make the pins according to the scheme.
3. Upload `main.ino` into your arduino

# Use sketch functions separately #

Repository has three functions:
* Central servo (Front Automatic Window)
* R\L Servos (Right/Left Fixate Servos)
* Stepper (Circle)

###### Central Servo ######
    move_doorservo(int mode) - mode (true/false)
    
    // (true) If the servo is in pos 90 (#define doorangle "degree of rotation") - servo returns to zero pos
    // (false) If the servo is in pos 0 (#define doorangle "degree of rotation") - servo returns to 90 pos
###### Stepper ######
    indexstepper(int tangaz, int mode) - tangaz (0-180 (position in degrees) ), mode (true/false (switch between servos) )
    
    // If we raise the servo - we can lower them, but not vice versa!
    // For swap this process, swap: mode == true & mode == false with steppermode = true; &  steppermode = false;
###### R\L Servos ######
    move_highservo(int mode) - mode (true/false)
    move_lowservo(int mode) - mode (true/false)
    
    // If we raise the servo - we can lower them, but not vice versa!
    // (true) lower
    // (false) raise by #define leftservo "degree of rotation"
    // This sketch contains 2 servos. Right and Left. Use as usual, raise - lowering.
    [ move_lowservo - right / move_highservo - left ]
