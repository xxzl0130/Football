#ifndef __DEFEND_MAIN_H__
#define __DEFEND_MAIN_H__

#include <arduino.h>
#include <math.h>
#include <stdlib.h>
#include <C:\Arduino\hardware\arduino\avr\libraries\EEPROM\EEPROM.h>
#include <C:\Arduino\hardware\arduino\avr\libraries\Wire\Wire.h>
#include <../HMC5883L/HMC5883L.h>
#include <../IR_Eye/IR_Eye.h>
#include <../DC_Motor/DC_Motor.h>
#include <../US_distance/US_distance.h>
#include <../AnalogGray/AnalogGray.h>
#include <../MsTimer2/MsTimer2.h>
#include <../Queue/Queue.h>

#ifndef uchr
#define uchr unsigned char
#endif

#ifndef uint
#define uint unsigned int
#endif

#ifndef ulong
#define ulong unsigned long
#endif

#define CompassStorageAddr      0
#define CompassStorageOffset    (3*2)

#ifndef NOP
#define NOP asm("nop")
#endif // NOP

extern HMC5883L            compass;
extern IR_Eye              eye;

extern DC_MotorPair<DC_Motor>   motor;

extern US_Distance         frontUS;

extern AnalogGray_Color    gray;

extern float xAxisMagDir;

void setCompassOffset(void);
void loadCompassOffset(void);
void setXAxisMagDir(void);
bool keyPressed(uchr pin,uchr mode = HIGH);

template <typename T>
T Avg(T *arr,uint n);
template <typename T>
T Sum(T *arr,uint n);
template <typename T>
T Max(T *arr,uint n);

void EEPROM_writeInt(uint addr,uint data);
uint EEPROM_readInt(uint addr);

void delay10ms();
void flashLED(uint time = 100);
#endif // __DEFEND_MAIN_H__
