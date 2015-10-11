#include<AttackMain.h>

void setup()
{
    MsTimer2::set(1000 / FLASH_FREQUENCE,process);
    //设置计时器，频率为FLASH_FREQUENCE
}
