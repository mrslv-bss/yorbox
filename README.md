![Image alt](https://github.com/BassTechnologies/CircleStorage/raw/master/img/Circle_Storage.png)
# About Circle Storage

>Circle Storage - portable box with circle mechanism.
It looks like a box closed on all sides, on the cover of which a display is located. Below the center place for the put/place hole.

>Auxiliary buttons are installed on the side.

# INSTALLATION // PINS #

If you have not changed the sketch pins, please, connect pins by instructions :

+ Sensor button - D2
+ Right servo - D5
+ Left servo - D6
+ Central servo - D4
+ Stepper - 8, 9, 10, 11
+ LCD with i2c - (SDA)	A4, (SCL)	A5

# Libraries #

Upload additional libraries from repository :
> https://github.com/BassTechnologies/CircleStorage/tree/master/libs

<hr>

Move uploaded libraries into
>> C:\Users\User\Documents\Arduino\libraries

or

>> x64 - C:\Program Files (x86)\Arduino\libraries

>> x32 - C:\Program Files\Arduino\libraries

<hr>

Additional libraries - CustomStepper & LiquidCrystal_I2C. Other libraries are standard

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

![Image alt](https://github.com/BassTechnologies/CircleStorage/raw/master/img/photo_2020-02-29_12-24-23.jpg)
![Image alt](https://github.com/BassTechnologies/CircleStorage/raw/master/img/photo_2020-02-29_12-30-04.jpg)
