#ifndef __ANALOG_GRAY_H__
#define __ANALOG_GRAY_H__

#include "arduino.h"

#ifndef uchr
#define uchr unsigned char
#endif

#ifndef uint
#define uint unsigned INT_MAX
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

#endif // __ANALOG_GRAY_H__
