#include "AttackMain.h"

template <typename T>
void xAxisRun(enum Dir,T Speed)
{
    xAxisMotor1.run(Dir,Speed);
    xAxisMotor2.run(Dir,Speed);
}

template <typename T>
void yAxisRun(enum Dir,T Speed)
{
    yAxisMotor1.run(Dir,Speed);
    yAxisMotor2.run(Dir,Speed);
}

template <typename T>
void rotateRun(enum Dir,T Speed)
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
