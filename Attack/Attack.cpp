#include <AttackMain.h>
#include "LSM303.h"
#include <Wire.h>
#include <MsTimer2.h>

#ifndef FLASH_FREQUENCE
#define FLASH_FREQUENCE 10
#endif

void setup()
{
    Wire.begin();
    compass.init();
    compass.enableDefault();

    MsTimer2::set((unsigned long)1000 / FLASH_FREQUENCE,process);
    MsTimer2::start();

    preset();
}

void loop()
{

}

void process()
{
}

