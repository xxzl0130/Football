#include "AttackMain.h"
#include <arduino.h>

HMC5883L        compass;
IR_Eye          eye();

DC_Motor_EN     xAxisMotor1(2,3,22);
DC_Motor_EN     xAxisMotor2(4,5,23);
DC_Motor_EN     yAxisMotor1(6,7,24);
DC_Motor_EN     yAxisMotor2(8,9,25);

US_Distance     xAxisUS1(26,27);
US_Distance     xAxisUS2(28,29);
US_Distance     yAxisUS1(30,31);
US_Distance     yAxisUS2(32,33);

AnalogGray_Color      xAxisGray1(A10,4);
AnalogGray_Color      xAxisGray2(A11,4);
AnalogGray_Color      yAxisGray1(A12,4);
AnalogGray_Color      yAxisGray2(A13,4);

inline void xAxisRun(Direction Dir,uchr Speed)
{
    xAxisMotor1.run(Dir,Speed);
    xAxisMotor2.run(Dir,Speed);
}

inline void yAxisRun(Direction Dir,uchr Speed)
{
    yAxisMotor1.run(Dir,Speed);
    yAxisMotor2.run(Dir,Speed);
}

inline void rotateRun(Direction Dir,uchr Speed)
{
    xAxisMotor1.run(Dir,Speed);
    xAxisMotor2.run(Dir == FORWORD ? BACKWORD : FORWORD,Speed);
    yAxisMotor1.run(Dir,Speed);
    yAxisMotor2.run(Dir == FORWORD ? BACKWORD : FORWORD,Speed);
}

inline void angleRun(float Angle,uchr Speed)
{
    float xAxisSpeed,yAxisSpeed;

    xAxisSpeed = cos(Angle);
    yAxisSpeed = sin(Angle);

    xAxisRun(xAxisSpeed > 0 ? FORWORD : BACKWORD,(uchr)fabs(Speed * xAxisSpeed));
    yAxisRun(yAxisSpeed > 0 ? FORWORD : BACKWORD,(uchr)fabs(Speed * yAxisSpeed));
}

void loadPresetColor()
{
    byte high,low;

    high = EEPROM.read(0);
    low = EEPROM.read(1);
    xAxisGray1.setColor(0,(uint)high << 8 | low);
    high = EEPROM.read(2);
    low = EEPROM.read(3);
    xAxisGray1.setColor(1,(uint)high << 8 | low);
    high = EEPROM.read(4);
    low = EEPROM.read(5);
    xAxisGray1.setColor(2,(uint)high << 8 | low);
    high = EEPROM.read(6);
    low = EEPROM.read(7);
    xAxisGray1.setColor(3,(uint)high << 8 | low);

    high = EEPROM.read(8);
    low = EEPROM.read(9);
    xAxisGray2.setColor(0,(uint)high << 8 | low);
    high = EEPROM.read(10);
    low = EEPROM.read(11);
    xAxisGray2.setColor(1,(uint)high << 8 | low);
    high = EEPROM.read(12);
    low = EEPROM.read(13);
    xAxisGray2.setColor(2,(uint)high << 8 | low);
    high = EEPROM.read(14);
    low = EEPROM.read(15);
    xAxisGray2.setColor(3,(uint)high << 8 | low);

    high = EEPROM.read(13);
    low = EEPROM.read(17);
    yAxisGray1.setColor(0,(uint)high << 8 | low);
    high = EEPROM.read(18);
    low = EEPROM.read(19);
    yAxisGray1.setColor(1,(uint)high << 8 | low);
    high = EEPROM.read(20);
    low = EEPROM.read(21);
    yAxisGray1.setColor(2,(uint)high << 8 | low);
    high = EEPROM.read(22);
    low = EEPROM.read(23);
    yAxisGray1.setColor(3,(uint)high << 8 | low);

    high = EEPROM.read(24);
    low = EEPROM.read(25);
    yAxisGray2.setColor(0,(uint)high << 8 | low);
    high = EEPROM.read(26);
    low = EEPROM.read(27);
    yAxisGray2.setColor(1,(uint)high << 8 | low);
    high = EEPROM.read(28);
    low = EEPROM.read(29);
    yAxisGray2.setColor(2,(uint)high << 8 | low);
    high = EEPROM.read(30);
    low = EEPROM.read(31);
    yAxisGray2.setColor(3,(uint)high << 8 | low);
}
