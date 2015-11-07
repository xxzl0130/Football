/*
 doing:调整defend
 todo:调整search chase attack  位置信息
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
void chaseBall(void);
bool isBall(void);

void setup()
{
    Wire.begin();
    compass.init();
    compass.enableDefault();

    //MsTimer2::set((uint)1000 / FLASH_FREQUENCE,chaseBall);
    //MsTimer2::start();

    //pinMode(3,INPUT);

#ifdef DEBUG
    debugSerial.begin(9600);
    debugSerial.println("debug");
#endif
    loadCompassOffset();
    loadPresetColor();
    preset();
}

void loop()
{
    while(!keyPressed(KeyPinSt + 7));
    defend();
    /*debugSerial.println(judgeArea());
    delay(500);*/
}

bool isBall(void)
{
    eye.getAllValue(value);
    return abs(eye.getMinValue(value) - eye.environIR) >= 16;
}

void move(void)
{
    float angle;

    eye.getAllValue(value);
    if(!isBall())
    {
        search();
    }
    else if(1)
    {
        //通过复眼对球位置判断和自身位置判断决定是进攻还是防守
        attack();
    }
    else
    {
        defend();
    }
}

void search(void)
{
    ulong time = millis();
    float angle;

    while(abs(eye.getMinValue(value) - eye.environIR) < 10)
    {
        motor.xAxis.run(FORWORD,192);
        motor.yAxis.rotateRun(FORWORD,255);
        //在场地画圆寻找球
        if(millis() - time > 5000)//5s后搜寻超时
        {
            defend();//回防
            return;
        }
    }

    delay(50);
    motor.stop();

    while(fabs((angle = getAngle2Ball()) - 45.0) > 10.0)
    {
        if(angle <= 180.0)
        {
            motor.rotateRun(FORWORD,128);
        }
        else
        {
            motor.rotateRun(BACKWORD,128);
        }
    }
    delay(100);
    motor.stop();
    anglePID.reset();
    chaseBall();
}

void chaseBall(void)
{
    static uchr flag = 0;
    angle = getAngle2Ball();
    if(flag)
    {
        motor.angleRun(angle,speed);
    }
    else
    {
        omega = max(abs(anglePID.Update(angle - 45.0,angle)),SPEED_MAX);
        if(angle <= 180.0)
        {
            if(motor.xAxis.right.getCurrentDir() == FORWORD)
            {
                motor.xAxis.right.run(FORWORD,omega);
                motor.xAxis.left.stop();
            }
            else
            {
                motor.xAxis.left.run(FORWORD,omega);
                motor.xAxis.right.stop();
            }
            if(motor.yAxis.left.getCurrentDir() == FORWORD)
            {
                motor.yAxis.left.run(FORWORD,omega);
                motor.yAxis.right.stop();
            }
            else
            {
                motor.yAxis.right.run(FORWORD,omega);
                motor.yAxis.left.stop();
            }
        }
        else
        {
            if(motor.xAxis.right.getCurrentDir() == BACKWORD)
            {
                motor.xAxis.right.run(BACKWORD,omega);
                motor.xAxis.left.stop();
            }
            else
            {
                motor.xAxis.left.run(BACKWORD,omega);
                motor.xAxis.right.stop();
            }
            if(motor.yAxis.left.getCurrentDir() == BACKWORD)
            {
                motor.yAxis.left.run(BACKWORD,omega);
                motor.yAxis.right.stop();
            }
            else
            {
                motor.yAxis.right.run(BACKWORD,omega);
                motor.yAxis.left.stop();
            }
        }
    }
    flag ^= 1;

#ifdef DEBUG
    debugSerial.print(angle);
    debugSerial.print("  ");
    debugSerial.println(omega);
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
        debugSerial.print(omega);
        debugSerial.print(" ");
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

    while(fabs(xAxisUS2.getDistance() - xAxisUS1.getDistance()) > 3.0)
    {
        motor.xAxis.run(dir,128);
    }
    motor.stop();
    while(yAxisUS1.getDistance() > 20.0)
    {
        motor.yAxis.run(BACKWORD,128);
    }
    motor.stop();
}

void attack()
{

}
