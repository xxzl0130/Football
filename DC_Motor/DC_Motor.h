#ifndef __DC_MOTOR_H__
#define __DC_MOTOR_H__

#include <arduino.h>

#ifndef uchr
#define uchr unsigned char
#endif

#define SPEED_MAX 255
#define SPEED_MIN 0

enum Direction{
    FORWORD,BACKWORD
};

class DC_Motor
{
protected:
    uchr Pin[2];
public:
    DC_Motor(uchr Pin0,uchr Pin1)
    {
        Pin[0] = Pin0;
        Pin[1] = Pin1;

        pinMode(Pin[0],OUTPUT);
        digitalWrite(Pin[0],LOW);

        pinMode(Pin[1],OUTPUT);
        digitalWrite(Pin[1],LOW);
    }

    void run(Direction Dir,uchr Speed)
    {
        analogWrite(Pin[Dir],Speed);
        digitalWrite(Pin[Dir ^ 1],LOW);
    }
/*
    void run(Direction Dir,float Speed)
    {
        analogWrite(Pin[Dir],map((long)(Speed * 100.0),0,10000L,0,255));
        digitalWrite(Pin[Dir ^ 1],LOW);
    }
*/
    void stop()
    {
        digitalWrite(Pin[0],HIGH);
        digitalWrite(Pin[1],HIGH);
    }
};

class DC_Motor_EN:public DC_Motor
{
protected:
    uchr EN_Pin;
public:
    DC_Motor_EN(uchr Pin0,uchr Pin1,uchr enPin):DC_Motor(Pin0,Pin1),EN_Pin(enPin)
    {
        pinMode(EN_Pin,OUTPUT);
        digitalWrite(EN_Pin,HIGH);
    }

    void enable()
    {
        digitalWrite(EN_Pin,HIGH);
    }

    void disable()
    {
        digitalWrite(EN_Pin,LOW);
    }
};

#endif //__DC_MOTOR_H__
