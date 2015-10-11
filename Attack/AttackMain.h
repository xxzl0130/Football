#ifndef __ATTACK_MAIN_H__
#define __ATTACK_MAIN_H__

#include <MyDef.h>
#include <arduino.h>
#include <math.h>
#include <C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire\Wire.h>
#include <../HMC5883L/HMC5883L.h>
#include <../IR_Eye/IR_Eye.h>
#include <../DC_Motor/DC_Motor.h>
#include <../US_distance/US_distance.h>
#include <../AnalogGray/AnalogGray.h>

void xAxisRun(Direction Dir,uchar Speed);
void yAxisRun(Direction Dir,uchar Speed);
void rotateRun(Direction Dir,uchar Speed);
void angleRun(float Angle,uchar Speed);

#endif // __ATTACK_MAIN_H__
