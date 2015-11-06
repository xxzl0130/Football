#ifndef __IR_EYE_H__
#define __IR_EYE_H__

#include "arduino.h"
#include "limits.h"
#include "string.h"

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
    uchr degreePerEye;
    /**每只眼对应的角度**/
    uint environIR;
    /**环境光干扰**/
    uint *value;

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

        value = new uint[cnt];
        memset(value,0,sizeof(uint) * cnt);
    }

/******************************
函数名称：printAll2Ser
函数功能：将所有通道的值打印至串口
传入参数：无
返回值：无
******************************/
#ifdef DEBUG
    void printAll2Ser();
#endif

/******************************
函数名称：getMinNo
函数功能：读取所有通道并返回值最小的通道编号
传入参数：无
返回值：返回值最小的通道编号
******************************/
    uchr getMinNo(void);
    uchr getMinNo(uint *arr);

/******************************
函数名称：getMinDir
函数功能：读取所有通道并计算值最小的通道所对应的角度
传入参数：无
返回值：值最小的通道所对应的角度
******************************/
    uint getMinDir(void);
    uchr getMinDir(uint *arr);

/******************************
函数名称：getAllValue
函数功能：读取所有通道的值
传入参数：用于存放数据的数组指针*arr
返回值：无
******************************/
    uint* getAllValue(uint *arr);
    inline uint* getAllValue(void);

/******************************
函数名称：getMinValue
函数功能：读取所有通道并返回值最小的值
传入参数：无
返回值：无
******************************/
    uint getMinValue(void);

    inline uint degreesPerEye(void) const;
    inline uint getEnvironIR(void) const;
    inline uint getCntEye(void) const;
    inline void setEnvironIR(uint IR);
};

#endif // __IR_EYE_H__
