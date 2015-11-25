/*
todo:
调整chaseBall
写attack
*/
#define DEBUG
#define debugSerial Serial1

#include <AttackMain.h>
#include "LSM303.h"
#include <Wire.h>
#include <MsTimer2.h>
#include <stdlib.h>

#ifndef FLASH_FREQUENCE
#define FLASH_FREQUENCE 25
#endif

#define holdBallThreshold   32
#define chaseBallThreshold  128

#define IntKey  3

#define NOP asm("nop")

uint value[12];
//存储复眼数据用的公共全局数组
uchr speed = 255,omega = 255;
float angle;
Direction dir = FORWORD;
volatile uchr powerState = 0;

void move(void);
void attack(void);
void defend(void);
void search(void);
void quickBack(void);
bool chaseBall(void);
bool isBall(void);
bool holdBall();
void rotate2XAxis(void);
void power();
void keepDoing(void);

void setup()
{
    Wire.begin();
    compass.init();
    compass.enableDefault();

#ifdef DEBUG
    debugSerial.begin(9600);
    debugSerial.println("debug");
#endif
    loadCompassOffset();
    loadPresetColor();
    preset();

    pinMode(IntKey,INPUT);
    attachInterrupt(1,power,HIGH);

    MsTimer2::set((uint)1000 / FLASH_FREQUENCE,keepDoing);
    MsTimer2::start();
}

void loop()
{
    if(powerState)
        move();
    else
        NOP;
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
    }
    else
    {
        if(!holdBall())
        {
            eye.getAllValue(value);
            if(eye.getMinValue(value) > chaseBallThreshold || ballFace2Enemy())
            {
                chaseBall();
            }
            else
            {
                quickBack();
            }
        }
        else
        {
            attack();
        }
    }
}

void search(void)
{
    ulong time;
    float angle;
#ifdef DEBUG
    debugSerial.println("search");
#endif
    time = millis();
    while(abs(eye.getMinValue(value) - eye.environIR) < 10)
    {
        motor.xAxis.run(FORWORD,255);
        motor.yAxis.rotateRun(FORWORD,192);
        //在场地画圆寻找球
        if(millis() - time > 1000)//1s后搜寻超时
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
    static uchr ch;
    eye.getAllValue(value);
    angle = getAngle2Ball(value);
    ch = eye.getMinNo(value);
    //holdBall();
    //改由定时器循环执行
    if(value[ch] < chaseBallThreshold && !(ch == 0 || ch == 1))
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
    rotate2XAxis();
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
    while(!inArea(1))
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
    chaseBall();
}

void quickBack(void)
{
#ifdef DEBUG
    debugSerial.println("quickBack");
#endif
    rotate2XAxis();
    while(yAxisUS1.getDistance() > 40.0)
    {
        motor.yAxis.run(BACKWORD,255);
        if(getAngle2Ball() <= 90.0)
        {
            break;
        }
    }
    motor.stop();
}

void rotate2XAxis(void)
{
    PID pid(0.1,0.005,0);
    while(fabsf((angle = compass.heading()) - xAxisMagDir) > 3.0)
    {
        omega = pid.Update(angle - xAxisMagDir,angle);
        motor.rotateRun(omega > 0 ? FORWORD : BACKWORD,min(255,fabsf(omega)));
    }
}

void power()
{
    if(keyPressed(IntKey))
    {
        powerState ^= 1;
        if(!powerState)
        {
            motor.stop();
        }
    }
#ifdef DEBUG
    debugSerial.print("power:");
    debugSerial.println(powerState);
#endif
}

void keepDoing()
{
    holdBall();
}
