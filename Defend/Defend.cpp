#include <DefendMain.h>
#include "HMC5883L.h"
#include <Wire.h>
#include <MsTimer2.h>
#include <stdlib.h>

/*#ifndef DEBUG
#define DEBUG
#define debugSerial Serial1
#endif*/

float dis,angle;
Direction dir;
uchr speed;
uint value[10];
void stop()
{
    motor.stop();
}

void setup() {
    // put your setup code here, to run once:
#ifdef DEBUG
    debugSerial.begin(9600);
    debugSerial.println("debug");
#endif
    Serial.begin(9600);
    pinMode(KeyPin,INPUT_PULLUP);
    pinMode(49,INPUT);
    attachInterrupt(1,stop,HIGH);
    if(digitalRead(49) == HIGH)
    {
        while(digitalRead(KeyPin) == HIGH);
        setCompassOffset();
    }
    else
    {
        loadCompassOffset();
    }
    while(digitalRead(KeyPin) == HIGH);
    setXAxisMagDir();
    motor.run(FORWORD,128);
}

void loop() {
  // put your main code here, to run repeatedly:
     dis = frontUS.getDistance();
     if(dis <= 60.0 || dis >= 88.0 || gray.read() < 110)
     {
        motor.reverse();
     }
#ifdef DEBUG
     eye.getAllValue(value);
     angle = eye.getMinDir(value);
     for(uint i = 0;i < 6;++i)
     {
        Serial.print(value[i]);
        Serial.print(" | ");
     }
     Serial.println(angle);
     Serial.println(gray.read());
#endif
     eye.getAllValue(value);
     angle = eye.getMinDir(value);
     if(angle <= 90.0)
     {
        dir = FORWORD;
     }
     else
     {
        dir = BACKWORD;
     }
     speed = map(eye.getMinValue(value),0,1024,255,0);
     motor.run(dir,speed);
#ifdef DEBUG
    Serial.print(dir);
    Serial.print(" ");
    Serial.println(speed);
#endif
     delay(50);
}
