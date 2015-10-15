#include <AttackMain.h>

#ifndef FLASH_FREQUENCE
#define FLASH_FREQUENCE 25
#endif

void setup()
{
    MsTimer2::set((unsigned long)1000 / FLASH_FREQUENCE,process);
}

void loop()
{
  
}

void process()
{
}

