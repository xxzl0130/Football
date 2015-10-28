#include <AttackMain.h>

#ifndef FLASH_FREQUENCE
#define FLASH_FREQUENCE 25
#endif

void setup()
{
    Wire.begin();
    compass.init();
    compass.enableDefault();

    MsTimer2::set((unsigned long)1000 / FLASH_FREQUENCE,process);
    MsTimer2::start();

    attachInterrupt(1,LOW,setXAxisMagDir);
}

void loop()
{

}

void process()
{
}

