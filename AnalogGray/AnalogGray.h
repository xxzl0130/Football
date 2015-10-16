#ifndef __ANALOG_GRAY_H__
#define __ANALOG_GRAY_H__

#include "arduino.h"
#include "Queue.h"
#include "stdlib.h"

#ifndef uchr
#define uchr unsigned char
#endif

#ifndef uint
#define uint unsigned int
#endif

#ifndef ColorThreshold
#define ColorThreshold      20
#endif

class AnalogGray{
protected:
    uchr pin;
public:
    AnalogGray(uchr p):pin(p)
    {
        pinMode(pin,INPUT);
    }
    uint read()
    {
        return analogRead(pin);
    }
};

class AnalogGray_Color:protected AnalogGray{
protected:
    uchr cntColor;
    uint *colorPtr;
    Queue::CircleQueue_Avg<uint> colorQue(8);
public:
    AnalogGray_Color(uchr p,uchr cnt):AnalogGray(p)
    {
        colorPtr = new uint[cnt];
    }

/******************************
�������ƣ�setColor
�������ܣ�����Ԥ����ɫ
�����������ɫ����ָ��*p
����ֵ����
******************************/
    void setColor(uint *p)
    {
        for(uint i = 0;i < cntColor;++i)
            *(colorPtr + i) = * (p + i);
    }
/******************************
�������ƣ�setColor
�������ܣ�����Ԥ����ɫ
�����������ɫ���k����ɫֵval
����ֵ����
******************************/
    void setColor(uchr k,uint val)
    {
        if(k >= cntColor - 1)
        {
            return;
        }
        *(colorPtr + k) = val;
    }

/******************************
�������ƣ�color
�������ܣ���ȡ�Ҷ�ֵ���ж���ɫ
�����������
����ֵ�����ϵ���ɫ��ţ��Ҳ����򷵻�0xff
******************************/
    uchr color()
    {
        uint val = read();
        for(uchr i = 0;i < cntColor;++i)
        {
            if(abs(val - *(colorPtr + i)) < ColorThreshold)
                return i;
        }
        return 0xff;
    }

/******************************
�������ƣ�smoothRead
�������ܣ�����CircleQueue_Avg�Զ�ȡ��ֵ����ƽ������
�����������
����ֵ��ƽ����������ɫֵ
******************************/
    uint smoothRead()
    {
        colorQue.push(read());
        return colorQue.avg();
    }
};

#endif // __ANALOG_GRAY_H__
