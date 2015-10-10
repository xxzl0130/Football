#ifndef __MY_MAIN_H__
#define __MY_MAIN_H__

#include "arduino.h"
#include "Wire.h"

#include "MyDef.h"
#include "HMC5883L.h"
#include "IR_Eye.h"
#include "DC_Motor.h"
#include "US_distance.h"
#include "AnalogGray.h"

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
AnalogGray      xAxisGray1(A12);
AnalogGray      xAxisGray2(A13);

#endif // __MY_MAIN_H__
