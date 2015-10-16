#include "AttackMain.h"
#include <arduino.h>

HMC5883L        compass;
IR_Eye          eye(A0,10,360);

DC_Motor_EN     xAxisMotor1(4,5,22);
DC_Motor_EN     xAxisMotor2(6,7,23);
DC_Motor_EN     yAxisMotor1(8,9,24);
DC_Motor_EN     yAxisMotor2(10,11,25);

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

inline void motorStop()
{
    xAxisMotor1.stop();
    xAxisMotor2.stop();
    yAxisMotor1.stop();
    yAxisMotor2.stop();
}

void presetColor()
{
    uint color[4][4];

    for(uchr i = 0;i < 10;++i)
    {
        xAxisGray1.smoothRead();
        xAxisGray2.smoothRead();
        yAxisGray1.smoothRead();
        yAxisGray1.smoothRead();
        delay(100);
    }
    //��ʼ��ƽ��ֵ

    color[0][0] = xAxisGray1.smoothRead();
    color[1][0] = xAxisGray2.smoothRead();
    color[2][0] = yAxisGray1.smoothRead();
    color[3][0] = yAxisGray2.smoothRead();
    //�����һ�����ݣ���Ҫ��֤���û����ڵ�һ�������������Ҳࣩɫ��

    do{
        xAxisRun(FORWORD,80);
        delay(40)
    }while(abs(color[0][0] - xAxisGray1.smoothRead()) <= ColorThreshold ||
           abs(color[1][0] - xAxisGray2.smoothRead()) <= ColorThreshold ||
           abs(color[2][0] - yAxisGray1.smoothRead()) <= ColorThreshold ||
           abs(color[3][0] - yAxisGray2.smoothRead()) <= ColorThreshold);
    delay(40);
    motorStop();
    //��һ�໺���ƶ���ͬʱ�����ɫ

    for(uchr i = 0;i < 8;++i)
    {
        xAxisGray1.smoothRead();
        xAxisGray2.smoothRead();
        yAxisGray1.smoothRead();
        yAxisGray1.smoothRead();
        delay(100);
    }
    //ƽ������

    color[0][1] = xAxisGray1.smoothRead();
    color[1][1] = xAxisGray2.smoothRead();
    color[2][1] = yAxisGray1.smoothRead();
    color[3][1] = yAxisGray2.smoothRead();
    //����ڶ�����ɫ

    do{
        xAxisRun(FORWORD,80);
        delay(40)
    }while(abs(color[0][1] - xAxisGray1.smoothRead()) <= ColorThreshold ||
           abs(color[1][1] - xAxisGray2.smoothRead()) <= ColorThreshold ||
           abs(color[2][1] - yAxisGray1.smoothRead()) <= ColorThreshold ||
           abs(color[3][1] - yAxisGray2.smoothRead()) <= ColorThreshold);
    delay(40);
    motorStop();
    //�����ƶ�Ѱ�ҵ�����ɫ��

    for(uchr i = 0;i < 8;++i)
    {
        xAxisGray1.smoothRead();
        xAxisGray2.smoothRead();
        yAxisGray1.smoothRead();
        yAxisGray1.smoothRead();
        delay(100);
    }
    //ƽ������

    color[0][2] = xAxisGray1.smoothRead();
    color[1][2] = xAxisGray2.smoothRead();
    color[2][2] = yAxisGray1.smoothRead();
    color[3][2] = yAxisGray2.smoothRead();
    //�����������ɫ

    do{
        xAxisRun(BACKWORD,80);
        delay(40)
    }while(abs(color[0][1] - xAxisGray1.smoothRead()) > ColorThreshold ||
           abs(color[1][1] - xAxisGray2.smoothRead()) > ColorThreshold ||
           abs(color[2][1] - yAxisGray1.smoothRead()) > ColorThreshold ||
           abs(color[3][1] - yAxisGray2.smoothRead()) > ColorThreshold);
    delay(40);
    motorStop();
    //�˻ص��ڶ���ɫ��
    do{
        yAxisRun(FORWORD,80);
        delay(40)
    }while(abs(color[0][1] - xAxisGray1.smoothRead()) <= ColorThreshold ||
           abs(color[1][1] - xAxisGray2.smoothRead()) <= ColorThreshold ||
           abs(color[2][1] - yAxisGray1.smoothRead()) <= ColorThreshold ||
           abs(color[3][1] - yAxisGray2.smoothRead()) <= ColorThreshold);
    delay(40);
    motorStop();
    //��ֱ�ƶ���������ɫ��

    for(uchr i = 0;i < 8;++i)
    {
        xAxisGray1.smoothRead();
        xAxisGray2.smoothRead();
        yAxisGray1.smoothRead();
        yAxisGray1.smoothRead();
        delay(100);
    }
    //ƽ������

    color[0][2] = xAxisGray1.smoothRead();
    color[1][2] = xAxisGray2.smoothRead();
    color[2][2] = yAxisGray1.smoothRead();
    color[3][2] = yAxisGray2.smoothRead();
    //������ĸ���ɫ

    uint addr = ColorStorageAddr;
    byte high,low;
    for(uchr i = 0;i < 4;++i)
    {
        for(uchr j = 0;j < 4;++j)
        {
            high = (byte)(color[i][j] >> 8);
            low = (byte)(color[i][j] & 0xff);
            EEPROM.write(addr++,high);
            EEPROM.write(addr+,low);
        }
    }
    //�����ݴ���EEPROM
}

void loadPresetColor()
{
    byte high,low;
    uint addr = ColorStorageAddr;

    for(uchr i = 0;i < 4;++i)
    {
        high = EEPROM.read(addr++);
        low = EEPROM.read(addr++);
        xAxisGray1.setColor(i,(uint)high << 8 | low);
    }

    for(uchr i = 0;i < 4;++i)
    {
        high = EEPROM.read(addr++);
        low = EEPROM.read(addr++);
        xAxisGray2.setColor(i,(uint)high << 8 | low);
    }

    for(uchr i = 0;i < 4;++i)
    {
        high = EEPROM.read(addr++);
        low = EEPROM.read(addr++);
        yAxisGray1.setColor(i,(uint)high << 8 | low);
    }

    for(uchr i = 0;i < 4;++i)
    {
        high = EEPROM.read(addr++);
        low = EEPROM.read(addr++);
        yAxisGray2.setColor(i,(uint)high << 8 | low);
    }
}
