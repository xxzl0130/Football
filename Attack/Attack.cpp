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
    //chaseBall();
    //move();
    chaseBall();
    delay(100);
}

bool isBall(void)
{
    eye.getAllValue(value);
    return eye.getMinValue(value) < eye.environIR - 16;
}

void move(void)
{
    float angle;
    static  unsigned long searchTime;
    bool searchFlag = false;

    if(isBall())
    {
        defend();
    }
    else
    {
        //通过复眼对球位置判断和自身位置判断决定是进攻还是防守
        attack();
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
        if(millis() - time > 2000)//2s后搜寻超时
        {
            defend();//回防
            return;
        }
    }

    delay(50);
    motor.stop();
    anglePID.reset();
    chaseBall();
}

bool chaseBall(void)
{
    static uchr flag = 0;
    //0 for xAxis,1 for yAxis.
    eye.getAllValue(value);
    eye.printAll2Ser(value);
    angle = getAngle2Ball(value);
    if(eye.getMinValue(value) < 128 && eye.getMinValue(value) > 64 && eye.getMinNo(value) != 0)
    {//较近距离
        /*if(angle > 45.0 && angle < 230.0)
        {
            motor.rotateRun(FORWORD,127);
        }
        else
        {
            motor.rotateRun(BACKWORD,127);
        }*/
        motor.rotateRun(FORWORD,100);
        return true;
    }
    else
    {
        motor.angleRun(angle,255);
        return false;
    }
#ifdef DEBUG
    debugSerial.println(angle);
    /*debugSerial.print("  ");
    debugSerial.println(omega);*/
#endif
}

void defend()
{
    uchr area;
    PID pid(0.4,0.005,0);
#ifdef DEBUG
    debugSerial.println("defend");
#endif

    while(fabs((angle = compass.heading()) - xAxisMagDir) > 3.0)
    {
        /*omega = max(abs(pid.Update(angle - xAxisMagDir,angle)),SPEED_MAX);
        motor.rotateRun(angle - xAxisMagDir > 0 ? FORWORD : BACKWORD,omega);
        delay(50);*/
        if(angle >= xAxisMagDir || angle + 360.0 - xAxisMagDir <= 180.0)
        {
            dir = BACKWORD;
        }
        else
        {
            dir = FORWORD;
        }
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
    chaseBall();
}
