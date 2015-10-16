#include "AttackMain.h"
#include <arduino.h>

HMC5883L            compass;
IR_Eye              eye(A0,6,180);

DC_Motor            lMotor(4,5);
DC_Motor            rMotor(6,7);

US_Distance         frontUS(8,9);

AnalogGray_Color    gray(A6);

