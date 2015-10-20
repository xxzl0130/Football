#ifndef __DC_MOTOR_H__
#define __DC_MOTOR_H__

#include <arduino.h>
#include <math.h>

#ifndef uchr
#define uchr unsigned char
#endif

#define SPEED_MAX 255
#define SPEED_MIN 0

enum Direction
{
    FORWORD = 0,BACKWORD
};

class DC_Motor
{
protected:
    uchr Pin[2];
    uchr currentSpeed;
    Direction currentDir;
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
        currentDir = Dir;
        currentSpeed = Speed;
        //存储当前的速度与方向
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

    void speedUp(int plusSpeed)
    {
        if(currentSpeed + plusSpeed < SPEED_MAX)
        {
            currentSpeed = SPEED_MAX;
        }
        else if(plusSpeed < 0 && -plusSpeed > currentSpeed)
        {
            currentSpeed = SPEED_MIN;
        }
        //保证加减后的速度还在0-255内

        run(currentDir,currentSpeed);
    }

    inline void reverse()
    {
        run(currentDir == FORWORD ? BACKWORD : FORWORD,currentSpeed);
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

    inline void enable()
    {
        digitalWrite(EN_Pin,HIGH);
    }

    inline void disable()
    {
        digitalWrite(EN_Pin,LOW);
    }
};

template <typename T>
class DC_MotorPair
{
private:
    T left,right;
public:
    DC_MotorPair(T a,T b):left(a),right(b)
    {
        //使用传入的DC_Motor_EN对象（或其基类DC_Motor对象）初始化
    }

    inline void run(Direction Dir,uchr Speed)
    {
        left.run(Dir,Speed);
        right.run(Dir,Speed);
    }

    inline void rotateRun(Direction Dir,uchr Speed)
    {
        left.run(Dir,Speed);
        right.run(Dir == FORWORD ? BACKWORD : FORWORD,Speed);
    }

    inline void reverse()
    {
        left.reverse();
        right.reverse();
    }

    inline void speedUp(int plusSpeed)
    {
        left.speedUp(plusSpeed);
        right.speedUp(plusSpeed);
    }

    inline void stop()
    {
        left.stop();
        right.stop();
    }
};

template <typename T>
class DC_MotorVerticalSquare
{
private:
    DC_MotorPair<T> xAxis,yAxis;
public:
    DC_MotorVerticalSquare(DC_MotorPair<T> x,DC_MotorPair<T> y):xAxis(x),yAxis(y)
    {
    }

    inline void xAxisRun(Direction Dir,uchr Speed)
    {
        xAxis.run(Dir,Speed);
    }

    inline void yAxisRun(Direction Dir,uchr Speed)
    {

        yAxis.run(Dir,Speed);
    }

    inline void angleRun(float Angle,uchr Speed)
    {
        float xAxisSpeed,yAxisSpeed;

        xAxisSpeed = cos(Angle);
        yAxisSpeed = sin(Angle);

        xAxis.Run(xAxisSpeed > 0 ? FORWORD : BACKWORD,(uchr)fabs(Speed * xAxisSpeed));
        yAxis.Run(yAxisSpeed > 0 ? FORWORD : BACKWORD,(uchr)fabs(Speed * yAxisSpeed));
    }

    inline void stop()
    {
        xAxis.stop();
        yAxis.stop();
    }

    inline void rotateRun(Direction Dir,uchr Speed)
    {
        xAxis.rotateRun(Dir,Speed);
        yAxis.rotateRun(Dir,Speed);
    }
};

#endif //__DC_MOTOR_H__
