/*
todo:
调整attack
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

#define holdBallThreshold   50
#define chaseBallThreshold  128

#define IntKey  2
#define modeKey 14
#define LED 13

#define NOP asm("nop")

uint value[12];
//存储复眼数据用的公共全局数组
uchr speed = SPEED_MAX;
int omega = 0;
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
void rotate2Gate(void);

void setup()
{
    Wire.begin();
    compass.init();
    compass.enableDefault();

    pinMode(IntKey,INPUT);
    pinMode(modeKey,INPUT_PULLUP);
    pinMode(LED,OUTPUT);

#ifdef DEBUG
    debugSerial.begin(9600);
    debugSerial.println("debug");
#endif
    loadCompassOffset();
    loadPresetColor();
    loadxAxisMagDir();

    if(digitalRead(modeKey) == HIGH)
    {
        preset();
    }
    else
    {
        powerState = 1;
    }

    attachInterrupt(0,power,HIGH);

    MsTimer2::set((uint)1000 / FLASH_FREQUENCE,keepDoing);
    MsTimer2::start();
}

void loop()
{
    if(powerState)
    {
        move();
    }
    else
    {
        motor.angleRun(0,0);
    }
}

bool isBall(void)
{
    return eye.getMinValue() < eye.environIR - 16;
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
            /*eye.getAllValue(value);
            if(eye.getMinValue(value) > chaseBallThreshold || ballFace2Enemy())*/
            {
                chaseBall();
            }
            /*else
            {
                quickBack();
            }*/
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
#ifdef DEBUG
    debugSerial.println("search");
#endif
    time = millis();
    while(!isBall())
    {
        motor.xAxis.run(FORWORD,SPEED_MAX);
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
        motor.angleRun(angle,SPEED_MAX);
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
        ballMotor.run(BACKWORD,SPEED_MAX);
        return true;
    }
    else
    {
        ballMotor.run(BACKWORD,map(val,holdBallThreshold,1024,SPEED_MAX,0));
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
    static Position<float> pos;
    static PID pid(0.5,0,0);
    pos = getCurPos();
#ifdef DEBUG
    debugSerial.print("(");
    debugSerial.print(pos.x);
    debugSerial.print(",");
    debugSerial.print(pos.y);
    debugSerial.print(") ");
    debugSerial.print(pos.angle);
    debugSerial.print(" ");
    debugSerial.print(pos.area);
    debugSerial.print(" ");
#endif
    if(pos.area = 0xff)
    {
        if(xAxisUS1.getDistance() < 20.0)
            motor.angleRun(90.0,128);
        else if(xAxisUS2.getDistance() < 20.0)
            motor.angleRun(-90.0,100);
        else if(yAxisUS1.getDistance() < 20.0)
            motor.angleRun(0,128);
        else if(yAxisUS2.getDistance() < 20.0)
            motor.angleRun(180.0,SPEED_MAX);
    }
    else if(face2Enemy(pos.angle))
    {
        if(pos.y > 160.0)
        {
            motor.angleRun(-getAngle2xAxis(pos.angle) - 90.0,128);
#ifdef DEBUG
            debugSerial.println("0");
#endif
        }
        else
        {
            if(fabsf(getAngle2xAxis(pos.angle)) < 10.0)
            {
                //接近正对
                if(fabsf(pos.x - halfMapWidth) > 20.0)
                {
                    //沒有居中
                    motor.angleRun(pos.x < halfMapWidth ? 0.0 : 180.0, SPEED_MAX);
#ifdef DEBUG
                    debugSerial.println("1");
#endif
                }
                else
                {
                    motor.angleRun(getAngle2Gate(pos) - getAngle2xAxis(pos.angle),SPEED_MAX);
                    omega = (int)pid.Update(getAngle2xAxis(pos.angle) - 45.0,pos.angle);
                    if(omega > 100)
                        omega = 100;
                    else if(omega < -100)
                        omega = -100;
                    motor.rotateSpeedUp(omega);
#ifdef DEBUG
                    debugSerial.println("2");
#endif
                }
            }
            else
            {
                motor.angleRun(getAngle2Gate(pos) - getAngle2xAxis(pos.angle),SPEED_MAX);
                omega = (int)pid.Update(getAngle2xAxis(pos.angle) - 45.0,pos.angle);
                if(omega > 100)
                    omega = 100;
                else if(omega < -100)
                    omega = -100;
                motor.rotateSpeedUp(omega);
#ifdef DEBUG
                debugSerial.println("3");
#endif
            }
        }
    }
    else
    {
        if(pos.y < 20.0)
        {
            motor.angleRun(getAngle2xAxis(pos.angle),128);
            //慢一点倒退带球
#ifdef DEBUG
            debugSerial.println("4");
#endif
        }
        else if(pos.y > 160.0)
        {
            //离底线太近，倒退回来
            motor.angleRun(getAngle2xAxis(pos.angle) - 180.0,100);
#ifdef DEBUG
            debugSerial.println("5");
#endif
        }
        else
        {
            //rotate2XAxis();
            motor.angleRun(getAngle2Gate(pos),SPEED_MAX);
            motor.rotateSpeedUp(2 * pid.Update(getAngle2xAxis(pos.angle),pos.angle));
#ifdef DEBUG
            debugSerial.println("6");
#endif
        }
    }
}

void quickBack(void)
{
#ifdef DEBUG
    debugSerial.println("quickBack");
#endif
    rotate2XAxis();
    while(yAxisUS1.getDistance() > 40.0)
    {
        motor.yAxis.run(BACKWORD,SPEED_MAX);
        if(getAngle2Ball() <= 90.0)
        {
            break;
        }
    }
    motor.stop();
}

void rotate2XAxis(void)
{
    PID pid(1,0.03,0);
    //redo:
    while(fabsf(getAngle2xAxis(angle = compass.heading())) > 10.0)
    {
        omega = (int)pid.Update(getAngle2xAxis(angle),angle);
#ifdef DEBUG
        debugSerial.print(getAngle2xAxis(angle));
        debugSerial.print(" ");
        debugSerial.println(omega);
#endif
        motor.rotateRun(omega > 0 ? FORWORD : BACKWORD,min(100,abs(omega)));
    }
#ifdef DEBUG
    debugSerial.print(getAngle2xAxis(angle));
    debugSerial.print(" ");
    debugSerial.println(omega);
#endif
    motor.stop();
    /*if(cosf(getAngle2xAxis()) < 0.95)
    {
        goto redo;
    }*/
}

void rotate2Gate(void)
{
    PID pid(0.05,0.005,0);
    while(fabsf(angle = getFaceAngle2Gate()) > 10.0)
    {
        omega = (int)pid.Update(angle - xAxisMagDir,angle);
        motor.rotateRun(omega > 0 ? FORWORD : BACKWORD,min(SPEED_MAX,abs(omega)));
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
            digitalWrite(LED,LOW);
        }
        else
        {
            digitalWrite(LED,HIGH);
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
