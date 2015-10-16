#ifndef __ATTACK_MAIN_H__
#define __ATTACK_MAIN_H__

#include <arduino.h>
#include <EEPROM.h>
#include <math.h>
#include <stdlib.h>
#include <C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire\Wire.h>
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

#ifndef ColorThreshold
#define ColorThreshold      20
#endif

#ifndef ColorStorageAddr
#define ColorStorageAddr    0
#endif

inline void xAxisRun(Direction Dir,uchr Speed);
inline void yAxisRun(Direction Dir,uchr Speed);
inline void rotateRun(Direction Dir,uchr Speed);
inline void angleRun(float Angle,uchr Speed);
inline void motorStop();

void process();
void presetColor();
void loadPresetColor();

#endif // __ATTACK_MAIN_H__
