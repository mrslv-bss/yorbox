![Image alt](https://github.com/BassTechnologies/CircleStorage/raw/master/img/Circle_Storage.png)
# Circle Storage - What is it?

>Circle Storage - The smart box with circle delivery system will surely amaze your friends and relatives.

>4 boxes of 7x5x1.5cm will allow you to store small details in a completely new way.
The carousel, which is located in the center of the structure, delivers your chosen slot with a box into a special, mechanical window.
The slot is selecting by pressing the touch button and displaying the current information on the LCD display.

>Very impressive.

>The design does not have its own box mounts, so for this a system of two servos in the “hooks” at the end was created.
When the structure is not using, the servos keep the boxes in place, which makes transporting the structure easy.

>Compared to other boxes, Circle Storage looks futuristic, expensive and technologically advanced.
Like every new product, Circle Storage also has its defects. But let's say directly - which technologies are not accompanied by defects in the early stages?

>Simple organizers - this is the last century, it's time to change. Let it be my challenge to mediocrity!

![Image alt](https://img.shields.io/badge/Sketch%20memory-34%25-green)

# How It Work #

Exit from hibernation is performed by a **single click** on the touch button.

Further, the display will show the text of the currently selected slot and the accompanying timer.

__1/4 slots__ can be selected with a single click on the touch button. Resetting the timer - holding the touch button.

After the timer expires, the two servos will go down, the central window will open and the carousel will be activated.

**The carousel scroll to the slot of your choice.**

To end the session with circle storage, perform a single press on the touch button.

The center window will close, the carousel will return to its starting position and the servos will lock the side boxes.

<hr>

A list of the main components we need:
1. LCD Display 2x16 (**x4 F-M adapter** (**x8** for wire extension))
2. Sensor button (**x3 F-M adapter** (**x6** for wire extension))
3. Servo (**x3**)
4. Stepper (**x1**)
5. Stepper driver (**x1**)
6. Arduino Nano/Uno (**x1**)

<hr>

###### YouTube ######

[<img src="https://img.youtube.com/vi/qYs0jXvUxDg/maxresdefault.jpg" width="150%">](https://youtu.be/qYs0jXvUxDg)

###### Remaster Edition (not released) ######
<img src="https://raw.githubusercontent.com/BassTechnologies/CircleStorage/master/img/118761516_111360574025919_1834713845074961977_o.jpg" width="150%">

###### Watch a video (download) ######
[<img src="https://raw.githubusercontent.com/BassTechnologies/CircleStorage/master/img/Screenshot_12.png" width="150%">](https://github.com/BassTechnologies/CircleStorage/blob/master/img/128597729_3788562921188669_8593680395783432028_n.mp4?raw=true)

# INSTALLATION // Getting Started #

If you have not changed the sketch pins, please, connect pins by instructions :

Sensor button | Right servo | Left servo | Central servo | Stepper | LCD with i2c
--- | --- | --- | --- | --- | ---
D2 | D5 | D6 | D4 | 8, 9, 10, 11 | (SDA) A4, (SCL) A5

![Image alt](https://github.com/BassTechnologies/CircleStorage/raw/master/img/Scheme.png)

# Libraries #

Upload additional [libraries](https://github.com/BassTechnologies/CircleStorage/tree/master/libs/ "Libraries") from repository.

Move uploaded libraries into:

| Documents       | x64                | x32 |
| ------------- |:------------------:| :------------------: |
| C:\Users\User\Documents\Arduino\libraries     | C:\Program Files (x86)\Arduino\libraries    | C:\Program Files\Arduino\libraries |

`Additional libraries - CustomStepper & LiquidCrystal_I2C. Other libraries are standard`

# Sketch #

1. Download `src/sketch/main.ino`
2. Make the pins according to the scheme.
3. Load `main.ino` into your arduino

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
>For commercial matters/Propose for Improvement code/scheme, write to the mail.
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

