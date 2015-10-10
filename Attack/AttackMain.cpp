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

template <typename T>
void xAxisRun(Direction Dir,T Speed)
{
    xAxisMotor1.run(Dir,Speed);
    xAxisMotor2.run(Dir,Speed);
}

template <typename T>
void yAxisRun(Direction Dir,T Speed)
{
    yAxisMotor1.run(Dir,Speed);
    yAxisMotor2.run(Dir,Speed);
}

template <typename T>
void rotateRun(Direction Dir,T Speed)
{
    xAxisMotor1.run(Dir,Speed);
    xAxisMotor2.run(Dir ^ 1,Speed);
    yAxisMotor1.run(Dir,Speed);
    yAxisMotor2.run(Dir ^ 1,Speed);
}

template <typename T>
void angleRun(float Angle,T Speed)
{
    float xAxisSpeed,yAxisSpeed;

    xAxisSpeed = cos(Angle);
    yAxisSpeed = sin(Angle);

    xAxisRun(xAxisSpeed > 0 ? FORWORD : BACKWORD,fabs(Speed * xAxisSpeed));
    yAxisRun(yAxisSpeed > 0 ? FORWORD : BACKWORD,fabs(Speed * yAxisSpeed));
}
