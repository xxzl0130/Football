#ifndef __ANALOG_GRAY_H__
#define __ANALOG_GRAY_H__

#include "arduino.h"

#ifndef uchr
#define uchr unsigned char
#endif

#ifndef uint
#define uint unsigned int
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
public:
    AnalogGray_Color(uchr p,uchr cnt):AnalogGray(p)
    {
        colorPtr = new uint[cnt];
    }

    void setColor(uint *p)
    {
        for(uint i = 0;i < cntColor;++i)
            *(colorPtr + i) = * (p + i);
    }

    void setColor(uchr k,uint val)
    {
        if(k >= cntColor - 1)
        {
            return;
        }
        *(colorPtr + k) = val;
    }

    uchr color()
    {
        uint val = read();
        for(uchr i = 0;i < cntColor;++i)
        {
            if(val <= *(colorPtr + i))
                return i;
        }
        return cntColor;
    }
};

#endif // __ANALOG_GRAY_H__
