#ifndef __ANALOG_GRAY_H__
#define __ANALOG_GRAY_H__

#include "arduino.h"
#include <../Queue/Queue.h>

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

class AnalogGray_Color:public AnalogGray{
protected:
    uchr cntColor;
    uint *colorPtr;
    CircleQueue_Avg<unsigned int> Que;
public:
    AnalogGray_Color(uchr p,uchr cnt):AnalogGray(p)
    {
        colorPtr = new uint[cnt];
    }

/******************************
函数名称：setColor
函数功能：加载预设颜色
传入参数：颜色数组指针*p
返回值：无
******************************/
    void setColor(uint *p)
    {
        for(uint i = 0;i < cntColor;++i)
            *(colorPtr + i) = * (p + i);
    }
/******************************
函数名称：setColor
函数功能：加载预设颜色
传入参数：颜色编号k，颜色值val
返回值：无
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
函数名称：color
函数功能：读取灰度值并判断颜色
传入参数：无
返回值：符合的颜色编号，找不到则返回0xff
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
函数名称：smoothRead
函数功能：利用CircleQueue_Avg对读取的值进行平滑处理
传入参数：无
返回值：平滑处理后的颜色值
******************************/
    uint smoothRead()
    {
        Que.push(read());
        return Que.avg();
    }
};

#endif // __ANALOG_GRAY_H__
