#ifndef __MY_MAIN_H__
#define __MY_MAIN_H__

#include <MyDef.h>
#include <arduino.h>
#include <math.h>
#include <C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire\Wire.h>
#include <../HMC5883L/HMC5883L.h>
#include <../IR_Eye/IR_Eye.h>
#include <../DC_Motor/DC_Motor.h>
#include <../US_distance/US_distance.h>
#include <../AnalogGray/AnalogGray.h>

template <typename T>
void xAxisRun(Direction Dir,T Speed);

template <typename T>
void yAxisRun(Direction Dir,T Speed);

template <typename T>
void rotateRun(Direction Dir,T Speed);

template <typename T>
void angleRun(float Angle,T Speed);

#endif // __MY_MAIN_H__
