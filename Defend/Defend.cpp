#include <AttackMain.h>
#include "HMC5883L.h"
#include <Wire.h>
#include <MsTimer2.h>
#include <stdlib.h>

void setup()
{
    pinMode(KeyPin,INPUT_PULLUP);
    setCompassOffset();
    while(!keyPressed(KeyPin,LOW));
    setXAxisMagDir();
}

void loop()
{

}
