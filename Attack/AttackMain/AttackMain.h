#ifndef __ATTACK_MAIN_H__
#define __ATTACK_MAIN_H__

#include <arduino.h>
#include <math.h>
#include <stdlib.h>
#include <C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM\EEPROM.h>
#include <C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire\Wire.h>
#include <../LSM303/LSM303.h>
#include <../IR_Eye/IR_Eye.h>
#include <../DC_Motor/DC_Motor.h>
#include <../US_distance/US_distance.h>
#include <../AnalogGray/AnalogGray.h>
#include <../MsTimer2/MsTimer2.h>
#include <../Queue/Queue.h>

#ifndef uchr
#define uchr unsigned char
#endif // uchr

#ifndef uint
#define uint unsigned int
#endif // uint

#ifndef ulong
#define ulong unsigned long
#endif // ulong

#define ColorThreshold          20
//不同颜色间的阈值
#define MaxDistance             250
#define MinDistance             5
//对超声波测距数据做上下限限制
#define MapLength               183
#define MapWidth                122
#define BlankWidth              30
#define SelfDiameter            24.5
//场地尺寸
#define ColorStorageAddr    0
#define ColorStorageOffset  (4*4*2)
#define CompassStorageAddr  (ColorStorageAddr + ColorStorageOffset)
#define CompassStorageOffset (3*2)
//EEPROM存储地址信息

#define degree2radian(angle) ((angle) / 180.0 * M_PI)

extern LSM303          compass;
extern IR_Eye          eye;

extern DC_MotorVerticalSquare<DC_Motor_EN> motor;

extern US_Distance     xAxisUS1;
extern US_Distance     xAxisUS2;
extern US_Distance     yAxisUS1;
extern US_Distance     yAxisUS2;

extern AnalogGray_Color      xAxisGray1;
extern AnalogGray_Color      xAxisGray2;
extern AnalogGray_Color      yAxisGray1;
extern AnalogGray_Color      yAxisGray2;

template<typename T>
struct Position{
    T x,y;
    float angle;
    Position()
    {
    }

    Position(T _x,T _y,float a = 0.0):x(_x),y(_y),angle(a)
    {
    }
};

void presetColor(void);
void loadPresetColor(void);
void setCompassOffset(void);
void loadCompassOffset(void);
void setXAxisMagDir(void);
//for preset

void flashLED(uint time = 200);
inline float checkDistance(float dis);

Position<float> getCurPosInfo(void);
float getAngle2Ball(void);
float getAngle2Gate(void);
float getAngle2xAxis(void);
void process(void);
#endif // __ATTACK_MAIN_H__
