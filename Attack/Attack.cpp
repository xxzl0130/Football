#include<AttackMain.h>

void setup()
{
    MsTimer2::set(1000 / FLASH_FREQUENCE,process);
    //���ü�ʱ����Ƶ��ΪFLASH_FREQUENCE
}
