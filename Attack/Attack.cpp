/*
 doing:调整chaseBall
 todo:调整sttack  位置信息
 */


#define DEBUG
#define debugSerial Serial

#include <AttackMain.h>
#include "LSM303.h"
#include <Wire.h>
#include <MsTimer2.h>
#include <stdlib.h>

#ifndef FLASH_FREQUENCE
#define FLASH_FREQUENCE 25
#endif

#define holdBallThreshold   32

uint value[12];
//存储复眼数据用的公共全局数组
uchr speed = 255,omega = 255;
float angle;
Direction dir = FORWORD;

PID anglePID(2.0,0.05,0);

void move(void);
void attack(void);
void defend(void);
void search(void);
bool chaseBall(void);
bool isBall(void);
bool holdBall();

void setup()
{
    Wire.begin();
    compass.init();
    compass.enableDefault();

    //pinMode(3,INPUT);

#ifdef DEBUG
    debugSerial.begin(9600);
    debugSerial.println("debug");
#endif
    loadCompassOffset();
    loadPresetColor();
    preset();

    /*MsTimer2::set((uint)1000 / FLASH_FREQUENCE,move);
    MsTimer2::start();*/
}

void loop()
{
    while(!keyPressed(keyPinSt + 7));
    while(1)
      move();
}

bool isBall(void)
{
    eye.getAllValue(value);
    return eye.getMinValue(value) < eye.environIR - 16;
}

void move(void)
{
    if(!isBall())
    {
        search();
        defend();
    }
    else
    {
        if(!holdBall())
        {
            chaseBall();
        }
        else
        {
            attack();
        }
    }
}

void search(void)
{
    ulong time = millis();
    float angle;

    while(abs(eye.getMinValue(value) - eye.environIR) < 10)
    {
        motor.xAxis.run(FORWORD,255);
        motor.yAxis.rotateRun(FORWORD,192);
        //在场地画圆寻找球
        if(millis() - time > 1000)//2s后搜寻超时
        {
            defend();//回防
            return;
        }
    }

    motor.stop();
}

bool chaseBall(void)
{
    //0 for xAxis,1 for yAxis.
    eye.getAllValue(value);
    angle = getAngle2Ball(value);
    holdBall();
    if(eye.getMinValue(value) < 128 && !holdBall())
    {
        //较近距离
        motor.rotateRun(FORWORD,128);
        return true;
    }
    else
    {
        motor.angleRun(angle,255);
        return false;
    }
#ifdef DEBUG
    /*debugSerial.println(angle);
    debugSerial.print("  ");
    debugSerial.println(omega);*/
#endif
}

bool holdBall()
{
    uint val;
    val = min(eye.read(0),eye.read(1));
#ifdef DEBUG
    /*debugSerial.print(val);
    debugSerial.print(" ");*/
#endif
    if(val <= holdBallThreshold)
    {
        ballMotor.run(BACKWORD,255);
        return true;
    }
    else
    {
        ballMotor.run(BACKWORD,map(val,holdBallThreshold,1024,255,0));
        return false;
    }
}

void defend()
{
    uchr area;
    PID pid(0.4,0.005,0);
#ifdef DEBUG
    debugSerial.println("defend");
#endif
    angle = compass.heading();
    if(angle >= xAxisMagDir || angle + 360.0 - xAxisMagDir <= 180.0)
    {
        dir = BACKWORD;
    }
    else
    {
        dir = FORWORD;
    }
    while(fabs(compass.heading() - xAxisMagDir) > 3.0)
    {
        motor.rotateRun(dir,128);
#ifdef DEBUG
        /*debugSerial.print(omega);
        debugSerial.print(" ");*/
        debugSerial.println(angle);
#endif
    }
    motor.stop();
    area = judgeArea();
#ifdef DEBUG
    debugSerial.println(area);
#endif
    if(xAxisUS2.getDistance() >= xAxisUS1.getDistance())
    {
        dir = FORWORD;
    }
    else
    {
        dir = BACKWORD;
    }
#ifdef DEBUG
    debugSerial.println(xAxisUS2.getDistance());
    debugSerial.println(xAxisUS2.getDistance());
#endif
    while(judgeArea() != 1)
    {
        motor.xAxis.run(dir,128);
    }
    motor.stop();
    while(!(yAxisGray2.color() == 1 && yAxisGray1.color() == 3) && yAxisUS1.getDistance() > 20.0)
    {
        motor.yAxis.run(BACKWORD,128);
    }
    motor.stop();
}

void attack()
{
    if(!holdBall())
    {
        chaseBall();
    }
    else
    {

    }
}
