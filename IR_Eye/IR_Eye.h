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
    uchr sPin;
    /**起始通道的接口，其余顺延**/
    uchr cntEye;
    /**复眼数目**/
    uint degreePerEye;
    /**每只眼对应的角度**/

public:
    IR_Eye(uchr Pin = A0,uchr Cnt = 6,uint Angel = 180)
    {
        sPin = Pin;
        cntEye = Cnt;

        for(uchr i = 0;i < cntEye;++i)
        {
            pinMode(sPin + i, INPUT);
        }

        degreePerEye = Angel / (cntEye - 1);
    }

/******************************
函数名称：getMinNo
函数功能：读取所有通道并返回值最小的通道编号
传入参数：无
返回值：返回值最小的通道编号
******************************/
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

    uchr getMinNo(uint *arr)
    {
        uint Min = INT_MAX;
        uchr no = 0;

        for(uchr i = 0;i < cntEye;++i)
        {
            if(arr[i] < Min)
            {
                Min = arr[i];
                no = i;
            }
        }

        return no;
    }

/******************************
函数名称：getMinDir
函数功能：读取所有通道并计算值最小的通道所对应的角度
传入参数：无
返回值：值最小的通道所对应的角度
******************************/
    uchr getMinDir()
    {
        return degreePerEye * getMinNo();
    }

    uchr getMinDir(uint *arr)
    {
        return degreePerEye * getMinNo(arr);
    }

/******************************
函数名称：getAllValue
函数功能：读取所有通道的值
传入参数：用于存放数据的数组指针*arr
返回值：无
******************************/
    void getAllValue(uint *arr)
    {
        for(uchr i = 0;i < cntEye;++i)
        {
            arr[i] = analogRead(sPin + i);
        }
    }

/******************************
函数名称：printAll2Ser
函数功能：将所有通道的值打印至串口
传入参数：无
返回值：无
******************************/
    void printAll2Ser()
    {
#ifdef debugSerial
        for(uchr i = 0;i < cntEye;++i)
        {
            debugSerial.print(analogRead(sPin + i));
            debugSerial.print(" | ");
        }
        debugSerial.print("\n");
#endif // debugSerial
    }

    inline uint degreesPerEye(void) const
    {
        return this -> degreePerEye;
    }
};

#endif // __IR_EYE_H__
