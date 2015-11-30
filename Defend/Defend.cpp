#include <DefendMain.h>
#include "HMC5883L.h"
#include <Wire.h>
#include <MsTimer2.h>
#include <stdlib.h>

#define IntKey  2
#define LED 13

/*#ifndef DEBUG
#define DEBUG
#define debugSerial Serial1
#endif*/

float dis,angle;
Direction dir;
uchr speed;
uint value[10];
volatile uchr powerState = 0;

void move();

void setup()
{
    // put your setup code here, to run once:
#ifdef DEBUG
    debugSerial.begin(9600);
    debugSerial.println("debug");
#endif
    pinMode(IntKey,INPUT_PULLUP);
    pinMode(49,INPUT);
    pinMode(LED,OUTPUT);
    digitalWrite(LED,LOW);
    if(digitalRead(49) == HIGH)
    {
        while(digitalRead(IntKey) == HIGH);
        setCompassOffset();
        flashLED();
    }
    else
    {
        loadCompassOffset();
        flashLED();
    }
    while(digitalRead(IntKey) == HIGH);
    setXAxisMagDir();
    flashLED();
    attachInterrupt(0,power,LOW);
}

void loop()
{
    // put your main code here, to run repeatedly:
    if(powerState)
        move();
    else
        NOP ;
}

void move()
{
    dis = frontUS.getDistance();
    if(dis <= 60.0 || dis >= 88.0 || gray.read() < 110)
    {
        motor.reverse();
    }
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

    //delay(50);
}

void power()
{
    if(keyPressed(IntKey))
    {
        powerState ^= 1;
        if(!powerState)
        {
            motor.stop();
            digitalWrite(LED,LOW);
        }
        else
        {
            digitalWrite(LED,HIGH);
        }
#ifdef DEBUG
        debugSerial.println(powerState);
#endif // DEBUG
    }
}
