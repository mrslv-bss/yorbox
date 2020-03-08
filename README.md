![Image alt](https://github.com/BassTechnologies/CircleStorage/raw/master/img/Circle_Storage.png)
# About Circle Storage

>Circle Storage - portable box with circle mechanism.
It looks like a box closed on all sides, on the cover of which a display is located. Below the center place for the put/place hole.

>Auxiliary buttons are installed on the side.

![Image alt](https://img.shields.io/badge/Sketch%20memory-34%25-green)

# How this work #
Algorithm

<hr>

At first, a list of the components we need:
1. LCD Display 2x16 (**x4 F-M adapter** (**x8** for wire extension))
2. Sensor button (**x3 F-M adapter** (**x6** for wire extension))
3. Servo (**x3**)
4. Stepper (**x1**)
5. Stepper driver (**x1**)

<hr>

# INSTALLATION // PINS #

If you have not changed the sketch pins, please, connect pins by instructions :

Sensor button | Right servo | Left servo | Central servo | Stepper | LCD with i2c
--- | --- | --- | --- | --- | ---
D2 | D5 | D6 | D4 | 8, 9, 10, 11 | (SDA) A4, (SCL) A5

![Image alt](https://github.com/BassTechnologies/CircleStorage/raw/master/img/Scheme.png)

# Libraries #

Upload additional libraries from repository :
> https://github.com/BassTechnologies/CircleStorage/tree/master/libs

Move uploaded libraries into

| Documents       | x64                | x32 |
| ------------- |:------------------:| :------------------: |
| C:\Users\User\Documents\Arduino\libraries     | C:\Program Files (x86)\Arduino\libraries    | C:\Program Files\Arduino\libraries |

`Additional libraries - CustomStepper & LiquidCrystal_I2C. Other libraries are standard`

# Use sketch functions separately #

Repository have three separately functions:
* Central servo (Front automatic window)
* R\L Servo's (Right/Left servo's for save boxs at fall)
* Stepper (Circle)

###### Central servo ######
    move_doorservo(int mode) - mode (true/false)
    
    // (true) If servo in pos 90 (#define doorangle "degree of rotation") - servo returns to zero pos
    // (false) If servo in pos 0 (#define doorangle "degree of rotation") - servo returns to 90 pos
###### Stepper ######
    indexstepper(int tangaz, int mode) - tangaz (0-180 (position in degrees) ), mode (true/false (switch between servos) )
    
    // The feature is that, if we raise servo - we can lowered them. But if we loose raise - lowered be impossible.
    // If you need swap this process, swap: mode == true & mode == false with steppermode = true; &  steppermode = false;
###### R\L Servo's ######
    move_highservo(int mode) - mode (true/false)
    move_lowservo(int mode) - mode (true/false)
    
    // ! The feature is that, if we raise servo - we can lowered them. But if we loose raise - lowered be impossible.
    // (true) lowering
    // (false) raise by #define leftservo "degree of rotation"
    // This sketch have 2 servo's. Right and Left. Use as usual, !!! raise - lowering !!!
    [ move_lowservo - right / move_highservo - left ]
  
***Contact us:***

>Visit our website to see other projects.
>
>www.bassmiroslav.pro
>
>For commercial matters, write to the mail.
>
><bassmiroslav@gmail.com>
  
# Production stages #

>![Image alt](https://github.com/BassTechnologies/CircleStorage/raw/master/img/photo_2020-03-07_16-28-32.jpg)
    
    Montage breadboard with installed on them Arduino Nano
    On the left and right side are installed servo
    Same, connected LED display, Stepper & Sens button.
>
>![Image alt](https://github.com/BassTechnologies/CircleStorage/raw/master/img/photo_2020-03-07_16-28-34.jpg)

    Stepper driver (keyestudio)
>
>![Image alt](https://github.com/BassTechnologies/CircleStorage/raw/master/img/photo_2020-03-07_16-28-35.jpg)
>
>![Image alt](https://github.com/BassTechnologies/CircleStorage/raw/master/img/photo_2020-03-07_16-28-37.jpg)

    The bridge by which the window closes the entry. 
    The servo without support from the post sags, which is why it is simply necessary.
>
>![Image alt](https://github.com/BassTechnologies/CircleStorage/raw/master/img/photo_2020-03-07_16-28-39.jpg)

    2x16 LCD
>
>![Image alt](https://github.com/BassTechnologies/CircleStorage/raw/master/img/photo_2020-02-29_12-24-23.jpg)
>
>![Image alt](https://github.com/BassTechnologies/CircleStorage/raw/master/img/photo_2020-02-29_12-30-04.jpg)

    Carousel. Created from a metal constructor.
>
### The End Result ###
![Image alt](https://github.com/BassTechnologies/CircleStorage/raw/master/img/circle_photo.png)

    Of course, maybe it doesn't look very good.
    But if a design and do it in plastic - something cool will come out at minimum!

