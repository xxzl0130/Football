#ifndef __ATTACK_MAIN_H__
#define __ATTACK_MAIN_H__

#include <arduino.h>
#include <math.h>
#include <stdlib.h>
#include <C:\Arduino\hardware\arduino\avr\libraries\EEPROM\EEPROM.h>
#include <C:\Arduino\hardware\arduino\avr\libraries\Wire\Wire.h>
#include <../LSM303/LSM303.h>
#include <../IR_Eye/IR_Eye.h>
#include <../DC_Motor/DC_Motor.h>
#include <../US_distance/US_distance.h>
#include <../AnalogGray/AnalogGray.h>
#include <../MsTimer2/MsTimer2.h>
#include <../Queue/Queue.h>
#include <../PID/PID.h>

#ifndef uchr
#define uchr unsigned char
#endif // uchr

#ifndef uint
#define uint unsigned int
#endif // uint

#ifndef ulong
#define ulong unsigned long
#endif // ulong

#define MaxDistance             250
#define MinDistance             5
//对超声波测距数据做上下限限制
#define MapLength               183
#define halfMapLength           91.5
#define MapWidth                122
#define halfMapWidth            61
#define yBlankWidth             30
#define xBlankWidth             22
#define SelfDiameter            24.5
#define halfSelfDiameter        12.25
#define GateDepth               8
//场地尺寸
#define ColorStorageAddr        0
#define ColorStorageOffset      (4*4*2)
#define CompassStorageAddr      (ColorStorageAddr + ColorStorageOffset)
#define CompassStorageOffset    (3*2)
#define xAxisStorageAddr        (CompassStorageAddr + CompassStorageOffset)
#define xAxisStorageOffset      (2)
//EEPROM存储地址信息
#define KeyPinSt                34
#define KeyPinCnt               8
//#define IntKey                  3

#define degree2radian(angle) ((angle) / 180.0 * M_PI)
#define radian2degree(angle) ((angle) / M_PI * 180.0)

#ifndef NOP
#define NOP asm("nop")
#endif // NOP

template<typename T>
struct Position
{
    T x,y;
    float angle;
    uchr area;
    Position()
    {
    }

    Position(T _x,T _y,float a = 0.0,uchr _area = 0):x(_x),y(_y),angle(a),area(_area)
    {
    }
};

extern LSM303          compass;
extern IR_Eye          eye;

extern DC_MotorVerticalSquare<DC_Motor_EN> motor;
extern DC_Motor_EN_1   ballMotor;

extern US_Distance     xAxisUS1;
extern US_Distance     xAxisUS2;
extern US_Distance     yAxisUS1;
extern US_Distance     yAxisUS2;

extern AnalogGray_Color      xAxisGray1;
extern AnalogGray_Color      xAxisGray2;
extern AnalogGray_Color      yAxisGray1;
extern AnalogGray_Color      yAxisGray2;

extern float                    xAxisMagDir;
extern const Position<float>    gatePosition,homePosition;

void presetColor(void);
void loadPresetColor(void);
void adjustColor(uchr);
void setCompassOffset(void);
void loadCompassOffset(void);
void setXAxisMagDir(void);
void loadxAxisMagDir(void);
void preset(void);
//for preset

bool keyPressed(uchr pin,uchr mode = HIGH);
void flashLED(uint time = 200);
float checkDistance(float);
uchr judgeArea(void);
bool inArea(uchr);
float getAngle2Ball(void);
float getAngle2Ball(uint*);
float getAngle2Gate(void);
float getAngle2Gate(Position<float> &pos);
float getAngle2xAxis(void);
float getAngle2xAxis(float);
float getAngle2Home(void);
float getFaceAngle2Gate(void);
Position<float> getCurPos(void);
bool face2Enemy(float);
bool face2Enemy(void);
bool ballFace2Enemy(void);
bool face2Ball(uint*);
bool face2Ball(void);

template <typename T>
T Avg(T *arr,uint n);
template <typename T>
T Sum(T *arr,uint n);
template <typename T>
T Max(T *arr,uint n);

void EEPROM_writeInt(uint addr,uint data);
uint EEPROM_readInt(uint addr);

void delay10ms();

#endif // __ATTACK_MAIN_H__
