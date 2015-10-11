#include "AttackMain.h"
#include <arduino.h>

HMC5883L        compass;
IR_Eye          eye(A0,10);

DC_Motor_EN     xAxisMotor1(2,3,22);
DC_Motor_EN     xAxisMotor2(4,5,23);
DC_Motor_EN     yAxisMotor1(6,7,24);
DC_Motor_EN     yAxisMotor2(8,9,25);

US_Distance     xAxisUS1(26,27);
US_Distance     xAxisUS2(28,29);
US_Distance     yAxisUS1(30,31);
US_Distance     yAxisUS2(32,33);

AnalogGray      xAxisGray1(A10);
AnalogGray      xAxisGray2(A11);
AnalogGray      yAxisGray1(A12);
AnalogGray      yAxisGray2(A13);

void xAxisRun(Direction Dir,uchr Speed)
{
    xAxisMotor1.run(Dir,Speed);
    xAxisMotor2.run(Dir,Speed);
}

void yAxisRun(Direction Dir,uchr Speed)
{
    yAxisMotor1.run(Dir,Speed);
    yAxisMotor2.run(Dir,Speed);
}

void rotateRun(Direction Dir,uchr Speed)
{
    xAxisMotor1.run(Dir,Speed);
    xAxisMotor2.run(Dir == FORWORD ? BACKWORD : FORWORD,Speed);
    yAxisMotor1.run(Dir,Speed);
    yAxisMotor2.run(Dir == FORWORD ? BACKWORD : FORWORD,Speed);
}

void angleRun(float Angle,uchr Speed)
{
    float xAxisSpeed,yAxisSpeed;

    xAxisSpeed = cos(Angle);
    yAxisSpeed = sin(Angle);

    xAxisRun(xAxisSpeed > 0 ? FORWORD : BACKWORD,(uchr)fabs(Speed * xAxisSpeed));
    yAxisRun(yAxisSpeed > 0 ? FORWORD : BACKWORD,(uchr)fabs(Speed * yAxisSpeed));
}
