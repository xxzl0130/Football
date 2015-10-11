#ifndef __IR_EYE_H__
#define __IR_EYE_H__

#include "arduino.h"
#include "limits.h"

#ifndef uchr
#define uchr unsigned char
#endif // uchr

#ifndef uint
#define uint unsigned int
#endif // uint

class IR_Eye{
private:
    uchr sPin, cntEye, degreePerEye;

public:
    IR_Eye(uchr Pin = A0,uchr Cnt = 6)
    {
        sPin = Pin;
        cntEye = Cnt;

        for(uchr i = 0;i < cntEye;++i)
        {
            pinMode(sPin + i, INPUT);
        }

        degreePerEye = 180 / (cntEye - 1);
    }

    uchr getMinNo()
    {
        uint Min = INT_MAX, tmp;
        uchr no = 0;

        for(uchr i = 0;i < cntEye;++i)
        {
            tmp = analogRead(sPin + i);
            if(tmp < Min)
            {
                Min = tmp;
                no = i;
            }
        }

        return no;
    }

    uchr getMinDir()
    {
        return degreePerEye * getMinNo();
    }

    void getAllValue(uint *arr)
    {
        for(uchr i = 0;i < cntEye;++i)
        {
            arr[i] = analogRead(sPin + i);
        }
    }
};

#endif // __IR_EYE_H__
